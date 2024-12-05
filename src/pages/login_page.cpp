#include "pages/login_page.h"
#include "application.h"
#include "pages/abstract_page.h"
#include "utils/utils.h"
#include <iostream>
#include <string>

constexpr const char *pin = "1234";

login_page::login_page() = default;

update_action login_page::update(application &app, std::ostream &cout,
                                 std::istream &cin) {
  utils::clear_screen(cout);

  if (!m_alert_msg.empty()) {
    cout << m_alert_msg;
  }

  cout << "Enter PIN: ";

  std::string inp;
  std::getline(cin, inp);
  utils::trim_string(inp);

  if (inp != pin) {
    m_alert_msg = "PIN doesn't match. Try again.\n";
    return update_action::none;
  }

  app.redirect("/");

  m_alert_msg.clear();
  return update_action::none;
}
