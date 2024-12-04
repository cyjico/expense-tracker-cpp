#pragma once
#include "pages/abstract_page.h"
#include <cstdint>
#include <iostream>
#include <string>
#include <unordered_map>

namespace search_expenses {

class by_datetime_page : public abstract_page {
  enum class state : uint8_t { prompt, further_prompt, end };
  state m_state = state::prompt;

  std::string m_prompt_message;
  std::unordered_map<std::string, std::string> m_prompt_cache;
  enum class option : uint8_t {
    find_day_with_highest_expense = 1,
    find_month_with_highest_expense,
    find_year_with_highest_expense
  };
  option m_selected_option;
  std::string m_search_result;

  void display_prompt(const application &app, std::ostream &cout);
  void handle_prompt(const application &app, const std::string &inp);

  state find_datetime_with_highest_expense(const application &app);
  static std::unordered_map<std::string, double>
  sum_expenses_by_datetime(const application &app, const bool &has_month,
                           const bool &has_day);

public:
  by_datetime_page();

  update_action update(application &app, std::ostream &cout,
                       std::istream &cin) override;
};

} // namespace search_expenses
