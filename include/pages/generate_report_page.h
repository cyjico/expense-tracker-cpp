#pragma once
#include "date.h"
#include "pages/abstract_page.h"
#include <cstdint>
#include <iostream>
#include <string>

class generate_report_page : public abstract_page {
  enum class state : uint8_t {
    prompt,
    end,
  };

  /**
   * @brief Represents the state of the page.
   */
  state m_state = state::prompt;
  std::string m_alert_msg;

  date m_date;

public:
  generate_report_page();

  update_action update(application &app, std::ostream &cout,
                       std::istream &cin) override;

  std::string handle_input(application &app, const std::string &inp);

  static void display_prompt(application &app, std::ostream &cout,
                             const state &state, const date &date);
};
