#include "pages/add_expense_page.h"
#include "application.h"
#include "expense.h"
#include "pages/abstract_page.h"
#include "utils/utils.h"
#include <iostream>
#include <regex>
#include <string>
#include <vector>

add_expense_page::add_expense_page() = default;

void add_expense_page::render(application & /*app*/, std::ostream &cout) {
  cout << "\x1B[2J\x1B[1;1H";

  cout << m_alert_msg;

  switch (m_state) {
  case state::prompt_date:
    cout << "Enter the date (dd/mm/yyyy): ";
    break;
  case state::prompt_category:
    cout << "Enter the category (Food, Transportation, etc.): ";
    break;
  case state::prompt_amount:
    cout << "Enter the amount: ";
    break;
  case state::prompt_desc:
    cout << "Enter a description (optional): ";
    break;
  case state::end:
  default:
    cout << "Expense added successfully!\nPress enter to return to the main "
            "menu.\n";
    break;
  }
}

update_action add_expense_page::update(application &app, std::istream &cin) {
  std::string inp;
  std::getline(cin, inp);
  inp = utils::trim_string(inp);

  m_alert_msg = "";

  switch (m_state) {
  case state::prompt_date: {
    // Matches dd/mm/yyyy
    const std::regex patt(R"(^([0-9]{2})/([0-9]{2})/([0-9]{4})$)");
    std::smatch match;

    if (!std::regex_match(inp, match, patt)) {
      m_alert_msg = "Invalid date format.\n";
      break;
    }

    m_expense.date.day = std::stoi(match[1].str());
    m_expense.date.month = std::stoi(match[2].str());
    m_expense.date.year = std::stoi(match[3].str());

    if (!m_expense.date.is_valid()) {
      m_alert_msg = "Invalid date range.\n";
      break;
    }

    m_state = state::prompt_category;
  } break;
  case state::prompt_category:
    if (inp.empty()) {
      m_alert_msg = "Category cannot be empty.\n";
      break;
    }

    m_expense.category = inp;
    m_state = state::prompt_amount;
    break;
  case state::prompt_amount: {
    // Matches num.num or num
    const std::regex patt(R"(^-?[0-9]+\.?[0-9]+$)");
    std::smatch match;

    if (!std::regex_match(inp, match, patt)) {
      m_alert_msg = "Invalid amount format.\n";
      break;
    }

    m_expense.amount = std::stod(match.str());

    if (m_expense.amount < 0) {
      m_alert_msg = "Amount cannot be negative.\n";
      break;
    }

    m_state = state::prompt_desc;
  } break;
  case state::prompt_desc:
    m_expense.desc = inp;
    m_state = state::end;
    break;
  case state::end:
  default: {
    app.at_shared_datum<std::vector<expense>>("expense").push_back(m_expense);

    app.redirect("/");
  } break;
  }

  return update_action::render_next_frame;
}
