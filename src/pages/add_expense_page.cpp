#include "pages/add_expense_page.h"
#include "application.h"
#include "expense.h"
#include "pages/abstract_page.h"
#include "utils/utils.h"
#include <iostream>
#include <regex>
#include <set>
#include <string>
#include <unordered_set>

add_expense_page::add_expense_page() = default;

update_action add_expense_page::update(application &app, std::ostream &cout,
                                       std::istream &cin) {
  cout << "\x1B[2J\x1B[1;1H" << std::flush;
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
  case state::prompt_category: {
    if (inp.empty()) {
      m_alert_msg = "Category cannot be empty.\n";
      break;
    }

    const auto &valid_categories =
        app.at_shared_datum<std::unordered_set<std::string>>(
            "valid_categories");

    if (valid_categories.find(inp) == valid_categories.end()) {
      std::string closest_category;
      double max_similarity = 0.0;

      for (const auto &category : valid_categories) {
        const double similarity = utils::jaro_winkler(inp, category);

        if (similarity > max_similarity) {
          max_similarity = similarity;
          closest_category = category;
        }
      }

      m_alert_msg = "Category does not exist.\n";
      if (max_similarity > 0.7) {
        m_alert_msg += "Did you perhaps mean \"" + closest_category + "\"?\n";
      }

      break;
    }

    m_expense.category = inp;
    m_state = state::prompt_amount;
  } break;
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
    app.at_shared_datum<std::multiset<expense>>("expenses").insert(m_expense);

    m_state = state::end;
    break;
  case state::end:
  default:
    app.redirect("/");

    m_state = state::prompt_date;
    break;
  }

  return update_action::none;
}
