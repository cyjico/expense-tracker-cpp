#include "pages/generate_report_page.h"
#include "application.h"
#include "date.h"
#include "expense.h"
#include "pages/abstract_page.h"
#include "utils/utils.h"
#include <ios>
#include <istream>
#include <ostream>
#include <regex>
#include <set>
#include <string>
#include <unordered_map>

generate_report_page::generate_report_page() = default;

update_action generate_report_page::update(application &app, std::ostream &cout,
                                           std::istream &cin) {
  utils::clear_screen(cout);
  cout << m_alert_msg;
  display_prompt(app, cout, m_state, m_date);

  std::string inp;
  std::getline(cin, inp);
  m_alert_msg = handle_input(app, utils::trim_string(inp));

  return update_action::none;
}

std::string generate_report_page::handle_input(application &app,
                                               const std::string &inp) {
  switch (m_state) {
  case state::prompt: {
    const std::regex patt(R"(^([0-9]?[0-9])\/([0-9]{1,4})$)");
    std::smatch match;

    if (!std::regex_match(inp, match, patt)) {
      return "Invalid date format.\n";
    }

    m_date.month = std::stoi(match[1].str());
    m_date.year = std::stoi(match[2].str());

    if (!m_date.is_valid()) {
      return "Invalid date range.\n";
    }

    m_state = state::end;
  } break;
  case state::end:
  default:
    app.redirect("/");

    m_state = state::prompt;
    break;
  }

  return "";
}

void generate_report_page::display_prompt(application &app, std::ostream &cout,
                                          const state &state,
                                          const date &date) {
  switch (state) {
  case state::prompt:
    cout << "Enter the month and year (mm/yyyy): ";
    break;
  case state::end:
  default: {
    double total_amount = 0.0;
    std::unordered_map<std::string, double> expenses_sum_by_category;

    auto expenses = app.at_shared_datum<application::expense_datum>("expenses");
    for (const auto &expense : expenses) {
      if (date.year != expense.date.year || date.month != expense.date.month) {
        continue;
      }

      total_amount += expense.amount;
      expenses_sum_by_category[expense.category] += expense.amount;
    }

    cout << "Monthly Report (" << date::get_name(date.month, date.year) << ")\n"
         << "---------------------------------"
         << "\nTotal Expenses: " << utils::double_to_string(total_amount)
         << '\n';

    for (const auto &expenses_sum : expenses_sum_by_category) {
      cout << expenses_sum.first << ": "
           << utils::double_to_string(expenses_sum.second) << " ("
           << (expenses_sum.second / total_amount * 100) << "%)\n";
    }

    cout << std::defaultfloat << "\nPress enter to return to the main menu.";
  } break;
  }
}
