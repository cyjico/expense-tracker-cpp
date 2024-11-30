#pragma once
#include "date.h"
#include "expense.h"
#include "pages/abstract_page.h"
#include <cstdint>
#include <istream>
#include <ostream>
#include <string>
#include <utility>
class application;

class add_expense_page : public abstract_page {
  enum class state : std::uint8_t {
    prompt_date,
    prompt_category,
    prompt_amount,
    prompt_desc,
    end,
  };
  state m_state = state::prompt_date;
  std::string m_alert_msg;

  expense m_expense;

  static std::pair<std::string, date> validate_date(const std::string &inp);
  static std::pair<std::string, std::string>
  validate_category(const application &app, const std::string &inp);
  static std::pair<std::string, double> validate_amount(const std::string &inp);

public:
  add_expense_page();

  update_action update(application &app, std::ostream &cout,
                       std::istream &cin) override;

  static void display_prompt(std::ostream &cout, const state &state);
  std::string handle_input(application &app, const std::string &inp,
                           expense &expense);
};
