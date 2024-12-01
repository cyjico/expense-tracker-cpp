#include "pages/search_expenses/by_category_page.h"
#include "application.h"
#include "date.h"
#include "expense.h"
#include "pages/abstract_page.h"
#include "utils/utils.h"
#include <algorithm>
#include <cstddef>
#include <exception>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace search_expenses {

namespace {

constexpr const char *default_prompt =
    "1. Find expenses under a category\n"
    "2. Find category with highest expense in a day\n"
    "3. Find category with highest expense in a month\n"
    "4. Find category with highest expense in a year\n";

} // namespace

void by_category_page::handle_prompt(const application &app,
                                     const std::string &inp) {
  switch (m_selected_option) {
  case option::find_expenses_under_category:
    m_state = find_expenses_under_a_category(app, inp);
    break;
  case option::find_category_with_highest_expense_in_day:
  case option::find_category_with_highest_expense_in_month:
  case option::find_category_with_highest_expense_in_year:
    m_state = find_category_with_highest_expense_in_datetime(app, inp);
    break;
  default:
    break;
  }
}

by_category_page::state
by_category_page::find_expenses_under_a_category(const application &app,
                                                 const std::string &inp) {
  // Prompt for category
  auto category_itr = m_prompt_cache.find("category");
  if (category_itr == m_prompt_cache.end()) {
    m_prompt_message = "What category?\n\n";
    m_prompt_cache["category"] = "";

    return state::further_prompt;
  }

  // Set category from the input
  if (category_itr->second.empty()) {
    // Check validity of category
    const auto &valid_categories =
        app.at_shared_datum<const std::unordered_set<std::string>>(
            "valid_categories");
    if (inp.empty() || valid_categories.find(inp) == valid_categories.end()) {
      std::string closest_category;
      double max_similarity = 0.0;

      for (const auto &category : valid_categories) {
        const double similarity = utils::jaro_winkler(inp, category);

        if (similarity > max_similarity) {
          max_similarity = similarity;
          closest_category = category;
        }
      }

      m_prompt_message =
          "Category \"" + inp + "\" does not exist.\n" +
          (max_similarity > 0.6
               ? "Did you perhaps mean \"" + closest_category + "\"?\n\n"
               : "\n");
      return state::further_prompt;
    }

    m_prompt_cache["category"] = inp;
  }

  // Prepare the search result
  m_search_result =
      "Expenses under category \"" + category_itr->second + "\" are:\n";

  const auto &expenses =
      app.at_shared_datum<application::expense_datum>("expenses");
  for (const auto &expense : expenses) {
    if (expense.category != category_itr->second) {
      continue;
    }

    m_search_result += "Spent " + utils::double_to_string(expense.amount) +
                       " at " + expense.date.to_string() + "\n";
  }

  // Remove the new line at the end
  m_search_result.erase(m_search_result.end() - 1);
  return state::end;
}

by_category_page::state
by_category_page::find_category_with_highest_expense_in_datetime(
    const application &app, const std::string &inp) {
  auto year_itr = m_prompt_cache.find("year");
  if (year_itr == m_prompt_cache.end()) {
    // Prompt for year
    m_prompt_message = "What year?\n\n";
    m_prompt_cache["year"] = "";

    return state::further_prompt;
  }

  if (year_itr->second.empty()) {
    // Set year from the input
    if (!validate_datetime_input("Year", 4, 1, 9999, inp)) {
      return state::further_prompt;
    }

    m_prompt_cache["year"] = inp;

    if (m_selected_option ==
            option::find_category_with_highest_expense_in_month ||
        m_selected_option ==
            option::find_category_with_highest_expense_in_day) {
      // Prompt for month
      m_prompt_message = "What month?\n\n";
      m_prompt_cache["month"] = "";

      return state::further_prompt;
    }
  }

  if ((m_selected_option ==
           option::find_category_with_highest_expense_in_month ||
       m_selected_option ==
           option::find_category_with_highest_expense_in_day) &&
      m_prompt_cache["month"].empty()) {
    // Set month from the input
    if (!validate_datetime_input("Month", 2, 1, 12, inp)) {
      return state::further_prompt;
    }

    m_prompt_cache["month"] = inp;

    if (m_selected_option ==
        option::find_category_with_highest_expense_in_day) {
      // Prompt for day
      m_prompt_message = "What day?\n\n";
      m_prompt_cache["day"] = "";

      return state::further_prompt;
    }
  }

  if (m_selected_option == option::find_category_with_highest_expense_in_day &&
      m_prompt_cache["day"].empty()) {
    // Set day from the input
    if (!validate_datetime_input("Day", 2, 1, 31, inp)) {
      return state::further_prompt;
    }

    m_prompt_cache["day"] = inp;
  }

  std::unordered_map<std::string, double> categoried_expenses =
      sum_expenses_by_category_in_datetime(app, m_prompt_cache["month"],
                                           m_prompt_cache["day"]);

  const std::string name = date::get_name(
      m_prompt_cache["day"].empty() ? 0 : std::stoi(m_prompt_cache["day"]),
      m_prompt_cache["month"].empty() ? 0 : std::stoi(m_prompt_cache["month"]),
      m_prompt_cache["year"].empty() ? 0 : std::stoi(m_prompt_cache["year"]));

  if (categoried_expenses.empty()) {
    m_search_result = "Found no expenses during " + name;
    return state::end;
  }

  auto max_iter = std::max_element(
      categoried_expenses.begin(), categoried_expenses.end(),
      [](const auto &lhs, const auto &rhs) { return lhs.second < rhs.second; });

  std::stringstream buffer;
  buffer << "Highest expense at " << name << " was in category \""
         << max_iter->first << "\" with an expense of "
         << utils::double_to_string(max_iter->second);

  m_search_result = buffer.str();
  return state::end;
}

// Skill issue if you swap these parameters
// NOLINTBEGIN(bugprone-easily-swappable-parameters)
bool by_category_page::validate_datetime_input(const std::string &name,
                                               size_t max_length, int min_value,
                                               int max_value,
                                               const std::string &inp) {
  if (inp.length() > max_length || inp.empty()) {
    m_prompt_message = name + " cannot be longer than " +
                       std::to_string(max_length) +
                       " characters nor empty.\n\n";
    return false;
  }

  int int_input = 0;
  try {
    int_input = std::stoi(inp);
  } catch (const std::exception &e) {
    m_prompt_message = "Please input a number. Try again.\n\n";
    return false;
  }

  if (int_input < min_value || int_input > max_value) {
    m_prompt_message = name + " must be between " + std::to_string(min_value) +
                       " and " + std::to_string(max_value) + ". Try again.\n\n";
    return false;
  }

  return true;
}
// NOLINTEND(bugprone-easily-swappable-parameters)

/**
 * @brief Calculates the sums of expenses grouped by categories within the
 * month/day.
 *
 * @param app
 * @param month If empty, will include any month.
 * @param day If empty, will include any day.
 * @return
 */
std::unordered_map<std::string, double>
by_category_page::sum_expenses_by_category_in_datetime(const application &app,
                                                       const std::string &month,
                                                       const std::string &day) {
  const auto &expenses =
      app.at_shared_datum<application::expense_datum>("expenses");
  std::unordered_map<std::string, double> result;

  for (const auto &expense : expenses) {
    if (!month.empty() && expense.date.month != std::stoi(month)) {
      continue;
    }

    if (!day.empty() && expense.date.day != std::stoi(day)) {
      continue;
    }

    result[expense.category] += expense.amount;
  }

  return result;
}

// --- public ---

by_category_page::by_category_page()
    : m_prompt_message(default_prompt),
      m_selected_option(option::find_expenses_under_category) {};

update_action by_category_page::update(application &app, std::ostream &cout,
                                       std::istream &cin) {
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

    m_selected_option = option::find_expenses_under_category;

    m_search_result = "";
    break;
  }

  return update_action::none;
}

} // namespace search_expenses
