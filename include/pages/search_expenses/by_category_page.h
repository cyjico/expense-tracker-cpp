#pragma once
#include "application.h"
#include "pages/abstract_page.h"
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string>
#include <unordered_map>

namespace search_expenses {

class by_category_page : public abstract_page {
  enum class state : uint8_t {
    prompt,
    further_prompt,
    end,
  };
  state m_state = state::prompt;

  std::string m_prompt_message;
  std::unordered_map<std::string, std::string> m_prompt_cache;
  enum class option : uint8_t {
    find_expenses_under_category = 1,
    find_category_with_highest_expense_in_day,
    find_category_with_highest_expense_in_month,
    find_category_with_highest_expense_in_year,
  };
  option m_selected_option;
  std::string m_search_result;

  void handle_prompt(const application &app, const std::string &inp);

  // --- `find_expenses_under_a_category` section ---
  state find_expenses_under_a_category(const application &app,
                                       const std::string &inp);

  // --- `find_category_with_highest_expense_in_datetime` section ---
  state find_category_with_highest_expense_in_datetime(const application &app,
                                                       const std::string &inp);
  bool validate_datetime_input(const std::string &name, size_t max_length,
                               int min_value, int max_value,
                               const std::string &inp);
  static std::unordered_map<std::string, double>
  sum_expenses_by_category_in_datetime(const application &app,
                                       const std::string &month,
                                       const std::string &day);

public:
  by_category_page();

  update_action update(application &app, std::ostream &cout,
                       std::istream &cin) override;
};

} // namespace search_expenses
