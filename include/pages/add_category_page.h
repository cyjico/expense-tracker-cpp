#pragma once
#include "pages/abstract_page.h"
#include <cstdint>
#include <iostream>
#include <string>

class add_category_page : public abstract_page {
  enum class state : uint8_t { prompt, end };
  state m_state = state::prompt;

  std::string m_alert_msg;
public:
  add_category_page();

  update_action update(application &app, std::ostream &cout,
                       std::istream &cin) override;
};
