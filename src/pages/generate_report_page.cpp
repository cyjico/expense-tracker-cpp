#include "pages/generate_report_page.h"
#include "application.h"
#include "date.h"
#include "expense.h"
#include "pages/abstract_page.h"
#include "utils/utils.h"
#include <iomanip>
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

  switch (m_state) {
  case state::prompt_date:
    cout << "Enter the month and year (mm/yyyy): ";
    break;
  case state::end:
  default: {
    double total_amount = 0.0;
    std::unordered_map<std::string, double> cat_amounts;

    auto expenses = app.at_shared_datum<std::multiset<expense>>("expenses");
    for (const auto &expense : expenses) {
      if (m_date.year != expense.date.year ||
          m_date.month != expense.date.month) {
        continue;
      }

      total_amount += expense.amount;

      if (cat_amounts.find(expense.category) == cat_amounts.end()) {
        cat_amounts.emplace(expense.category, 0.0);
      }

      cat_amounts.at(expense.category) += expense.amount;
    }

    cout << "Monthly Report (" << date::get_month_name(m_date.month) << " "
         << m_date.year << ")\n"
         << "---------------------------------"
         << "\nTotal Expenses: " << total_amount << '\n'
         << std::fixed << std::setprecision(2);

    for (const auto &cat_amount : cat_amounts) {
      cout << cat_amount.first << ": " << cat_amount.second << " ("
           << (cat_amount.second / total_amount * 100) << "%)\n";
    }

    cout << std::defaultfloat << "\nPress enter to return to the main menu.";
  } break;
  }

  std::string inp;
  std::getline(cin, inp);
  inp = utils::trim_string(inp);

  m_alert_msg = "";
  switch (m_state) {
  case state::prompt_date: {
    // Matches dd/mm/yyyy
    const std::regex patt(R"(^([0-9]{2})/([0-9]{4})$)");
    std::smatch match;

    if (!std::regex_match(inp, match, patt)) {
      m_alert_msg = "Invalid date format.\n";
      break;
    }

    m_date.month = std::stoi(match[1].str());
    m_date.year = std::stoi(match[2].str());

    if (!m_date.is_valid()) {
      m_alert_msg = "Invalid date range.\n";
      break;
    }

    m_state = state::end;
  } break;
  case state::end:
  default:
    app.redirect("/");

    m_state = state::prompt_date;
    break;
  }

  return update_action::none;
}
