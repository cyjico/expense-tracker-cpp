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
  int m_selected_option;
  std::string m_search_result;

  void handle_prompt(application &app, const std::string &inp);

  // --- `find_expenses_under_a_category` gang ---
  state find_expenses_under_a_category(application &app,
                                       const std::string &inp);

  // --- `find_category_with_highest_expense_in_timeframe` gang ---
  state find_category_with_highest_expense_in_timeframe(application &app,
                                                        const std::string &inp);
  bool validate_datetime_input(const std::string &name, size_t max_length,
                               int min_value, int max_value,
                               const std::string &inp);
  static std::unordered_map<std::string, double>
  filter_expenses_by_date(const application &app, const std::string &month,
                          const std::string &day);

public:
  by_category_page();

  update_action update(application &app, std::ostream &cout,
                       std::istream &cin) override;
};

} // namespace search_expenses
