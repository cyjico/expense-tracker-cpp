#include "pages/search_expenses/by_datetime_page.h"
#include "application.h"
#include "date.h"
#include "expense.h"
#include "pages/abstract_page.h"
#include "utils/utils.h"
#include <algorithm>
#include <exception>
#include <iostream>
#include <set>
#include <string>
#include <unordered_map>
#include <utility>

namespace search_expenses {

namespace {

constexpr const char *default_prompt =
    "1. Find a specific day with the highest expense\n"
    "2. Find a specific month with the highest expense\n"
    "3. Find a specific year with the highest expense\n";

} // namespace

void by_datetime_page::display_prompt(const application & /*app*/,
                                      std::ostream &cout) {
  utils::clear_screen(cout);

  switch (m_state) {
  case state::prompt:
  case state::further_prompt:
    cout << m_prompt_message;
    break;
  case state::end:
    cout << m_search_result << "\n\nPress enter to return to the main menu.";
    break;
  }
}

namespace {

// NOLINTBEGIN(bugprone-easily-swappable-parameters)
/**
 * @brief Retrieve the a key based on the date and datetime provided.
 *
 * Helper function used to group expenses by the date using the key calculated.
 *
 * @param date
 * @param has_month If true, will include month.
 * @param has_day If true, even if `has_month` is false, will also include
 * month.
 * @return std::string
 */
std::string get_datetime_key(const date &date, const bool &has_month = false,
                             const bool &has_day = false) {
  if (has_day) {
    return ((date.day < 10 ? "0" : "") + std::to_string(date.day)) + "/" +
           ((date.month < 10 ? "0" : "") + std::to_string(date.month)) + "/" +
           std::to_string(date.year);
  }

  if (has_month) {
    return "00/" + ((date.month < 10 ? "0" : "") + std::to_string(date.month)) +
           "/" + std::to_string(date.year);
  }

  return "00/00/" + std::to_string(date.year);
}
// NOLINTEND(bugprone-easily-swappable-parameters)

} // namespace

void by_datetime_page::handle_prompt(const application &app,
                                     const std::string & /*inp*/) {
  switch (m_selected_option) {
  case option::find_day_with_highest_expense:
  case option::find_month_with_highest_expense:
  case option::find_year_with_highest_expense:
    m_state = find_datetime_with_highest_expense(app);
    break;
  }
}

by_datetime_page::state
by_datetime_page::find_datetime_with_highest_expense(const application &app) {
  std::unordered_map<std::string, double> expenses = sum_expenses_by_datetime(
      app, m_selected_option == option::find_month_with_highest_expense,
      m_selected_option == option::find_day_with_highest_expense);

  auto max_iter =
      std::max_element(expenses.begin(), expenses.end(),
                       [](const std::pair<std::string, double> &lhs,
                          const std::pair<std::string, double> &rhs) -> bool {
                         return lhs.second < rhs.second;
                       });

  const date temp_date = date::from_string(max_iter->first);
  m_search_result =
      "The datetime with highest expense was at " +
      date::get_name(temp_date.day, temp_date.month, temp_date.year) +
      " with an expense totaling up to " +
      utils::double_to_string(max_iter->second);

  return state::end;
}

// NOLINTBEGIN(bugprone-easily-swappable-parameters)
/**
 * @brief Sums expenses based on the datetime.
 *
 * @param app
 * @param has_month If true, will sum the expenses by month and year.
 * @param has_day If true, even if `has_month` is false, will sum the expenses
 * by the day, month, and year.
 * @return std::unordered_map<std::string, double>
 */
std::unordered_map<std::string, double>
by_datetime_page::sum_expenses_by_datetime(const application &app,
                                           const bool &has_month,
                                           const bool &has_day) {
  const auto &expenses =
      app.at_shared_datum<std::multiset<expense>>("expenses");
  std::unordered_map<std::string, double> result;

  for (const auto &expense : expenses) {
    result[get_datetime_key(expense.date, has_month, has_day)] +=
        expense.amount;
  }

  return result;
}
// NOLINTEND(bugprone-easily-swappable-parameters)

// --- public ---

by_datetime_page::by_datetime_page()
    : m_prompt_message(default_prompt),
      m_selected_option(option::find_day_with_highest_expense) {}

update_action by_datetime_page::update(application &app, std::ostream &cout,
                                       std::istream &cin) {
  display_prompt(app, cout);

  std::string inp;
  std::getline(cin, inp);
  inp = utils::trim_string(inp);

  switch (m_state) {
  case state::prompt:
    try {
      m_selected_option =
          static_cast<option>(std::stoi(utils::trim_string(inp)));
    } catch (const std::exception &e) {
      break;
    }

    // DON'T add a `break;` because fall-through is intended!
  case state::further_prompt:
    handle_prompt(app, inp);
    break;
  case state::end:
    app.redirect("/");

    // Reset variables for the next time the page is loaded
    m_state = state::prompt;

    m_prompt_message = default_prompt;
    m_prompt_cache.clear();

    m_selected_option = option::find_day_with_highest_expense;

    m_search_result = "";
    break;
  }

  return update_action::none;
}

} // namespace search_expenses
