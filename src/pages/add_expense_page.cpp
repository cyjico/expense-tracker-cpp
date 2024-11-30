#include "pages/add_expense_page.h"
#include "application.h"
#include "date.h"
#include "expense.h"
#include "pages/abstract_page.h"
#include "utils/utils.h"
#include <iostream>
#include <regex>
#include <set>
#include <string>
#include <unordered_set>
#include <utility>

std::pair<std::string, date>
add_expense_page::validate_date(const std::string &inp) {
  date second;

  const std::regex patt(R"(^([0-9]{2})/([0-9]{2})/([0-9]{4})$)");
  std::smatch match;

  if (!std::regex_match(inp, match, patt)) {
    return std::make_pair("Invalid date format.\n", second);
  }

  second.day = std::stoi(match[1].str());
  second.month = std::stoi(match[2].str());
  second.year = std::stoi(match[3].str());

  if (!second.is_valid()) {
    return std::make_pair("Invalid date range.\n", second);
  }

  return std::make_pair("", second);
}

std::pair<std::string, std::string>
add_expense_page::validate_category(const application &app,
                                    const std::string &inp) {
  if (inp.empty()) {
    return std::make_pair("Category cannot be empty.", "");
  }

  const auto &valid_categories =
      app.at_shared_datum<std::unordered_set<std::string>>("valid_categories");

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

    const std::string buffer =
        "Category \"" + inp + "\" does not exist.\n" +
        (max_similarity > 0.6
             ? "Did you perhaps mean \"" + closest_category + "\"?\n\n"
             : "\n\n");

    return std::make_pair(buffer, "");
  }

  return std::make_pair("", inp);
}

std::pair<std::string, double>
add_expense_page::validate_amount(const std::string &inp) {
  const std::regex patt(R"(^-?[0-9]+\.?[0-9]+$)");
  std::smatch match;

  if (!std::regex_match(inp, match, patt)) {
    return std::make_pair("Invalid amount format.\n", 0);
  }

  const double amount = std::stod(match.str());

  if (amount < 0) {
    return std::make_pair("Amount cannot be negative.\n", 0);
  }

  return std::make_pair("", amount);
}

add_expense_page::add_expense_page() = default;

update_action add_expense_page::update(application &app, std::ostream &cout,
                                       std::istream &cin) {
  utils::clear_screen(cout);
  cout << m_alert_msg;
  display_prompt(app, cout, m_state);

  std::string inp;
  std::getline(cin, inp);
  m_alert_msg = handle_input(app, utils::trim_string(inp));

  return update_action::none;
}

std::string add_expense_page::handle_input(application &app,
                                           const std::string &inp) {
  switch (m_state) {
  case state::prompt_date: {
    const std::pair pair = validate_date(inp);
    if (!pair.first.empty()) {
      return pair.first;
    }

    m_expense.date = pair.second;
    m_state = state::prompt_category;
  } break;
  case state::prompt_category: {
    const std::pair pair = validate_category(app, inp);
    if (!pair.first.empty()) {
      return pair.first;
    }

    m_expense.category = pair.second;
    m_state = state::prompt_amount;
  } break;
  case state::prompt_amount: {
    const std::pair pair = validate_amount(inp);
    if (!pair.first.empty()) {
      return pair.first;
    }

    m_expense.amount = pair.second;
    m_state = state::prompt_desc;
  } break;
  case state::prompt_desc:
    m_expense.desc = inp;
    app.at_shared_datum<std::multiset<struct expense>>("expenses")
        .insert(m_expense);

    m_state = state::end;
    break;
  case state::end:
  default:
    app.redirect("/");

    m_state = state::prompt_date;
    break;
  }

  return "";
}

void add_expense_page::display_prompt(const application &app,
                                      std::ostream &cout, const state &state) {
  switch (state) {
  case state::prompt_date:
    cout << "Enter the date (dd/mm/yyyy): ";
    break;
  case state::prompt_category:
    cout << "The availble categories are:\n";

    {
      const auto &valid_categories =
          app.at_shared_datum<std::unordered_set<std::string>>(
              "valid_categories");

      for (const auto &category : valid_categories) {
        cout << category << '\n';
      }
    }

    cout << "\nEnter the category: ";
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
