#include "pages/add_expense_page.h"
#include "abstract_page.h"
#include "application.h"
#include "util.h"
#include <iostream>
#include <regex>
#include <sstream>
#include <string>

add_expense_page::add_expense_page() : m_date(0, 0, 0), m_amount(0) {};

void add_expense_page::render(std::ostream &cout) {
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
  inp = util::trim_string(inp);

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

    m_date.day = std::stoi(match[1].str());
    m_date.month = std::stoi(match[2].str());
    m_date.year = std::stoi(match[3].str());

    if (!m_date.is_valid()) {
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

    m_category = inp;
    m_state = state::prompt_amount;
    break;
  case state::prompt_amount: {
    // Matches num.num or num
    const std::regex patt(R"(^[0-9]+\.?[0-9]+$)");
    std::smatch match;

    if (!std::regex_match(inp, match, patt)) {
      m_alert_msg = "Invalid amount format.\n";
      break;
    }

    m_amount = std::stof(match.str());
    m_state = state::prompt_desc;
  } break;
  case state::prompt_desc:
    m_desc = inp;
    m_state = state::end;
    break;
  case state::end:
  default: {
    std::ostringstream oss;
    oss << m_date.to_string() << "," << m_category << "," << m_amount << ","
        << m_desc;

    app.insert_or_assign_shared_data("new_expense", oss.str());
    app.redirect("/");
  } break;
  }

  return update_action::render_next_frame;
}
