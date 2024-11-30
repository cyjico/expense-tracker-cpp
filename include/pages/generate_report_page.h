#pragma once
#include "date.h"
#include "pages/abstract_page.h"
#include <cstdint>
#include <iostream>
#include <string>

class generate_report_page : public abstract_page {
  enum class state : uint8_t {
    prompt_date,
    end,
  };
  state m_state = state::prompt_date;
  std::string m_alert_msg;

  date m_date;

public:
  generate_report_page();

  void render(application &app, std::ostream &cout) override;
  update_action update(application &app, std::istream &cin) override;
};