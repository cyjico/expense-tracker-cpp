#pragma once
#include "pages/abstract_page.h"
#include <iostream>
#include <string>

class login_page : public abstract_page {
  std::string m_alert_msg;

public:
  login_page();

  update_action update(application &app, std::ostream &cout,
                       std::istream &cin) override;
};
