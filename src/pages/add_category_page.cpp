#include "pages/add_category_page.h"
#include "application.h"
#include "pages/abstract_page.h"
#include "utils/utils.h"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>

add_category_page::add_category_page() = default;

update_action add_category_page::update(application &app, std::ostream &cout,
                                        std::istream &cin) {
  utils::clear_screen(cout);

  switch (m_state) {
  case state::prompt:
    if (!m_alert_msg.empty()) {
      cout << m_alert_msg;
    }

    cout << "Category name: ";
    break;
  case state::end:
  default:
    cout << "Category created successfully!\nPress enter to return to the main "
            "menu.\n";
    break;
  }

  std::string inp;
  std::getline(cin, inp);
  utils::trim_string(inp);

  switch (m_state) {
  case state::prompt: {
    // Check if input is empty
    if (inp.empty()) {
      m_alert_msg = "Category name can't be empty.\n";
      break;
    }

    auto &valid_categories =
        app.at_shared_datum<application::valid_categories_datum>(
            "valid_categories");

    // Case-insensitive check for pre-existing categories
    std::string inp_lower = inp;
    std::transform(
        inp_lower.begin(), inp_lower.end(), inp_lower.begin(),
        [](unsigned char param_char) { return std::tolower(param_char); });

    auto itr = std::find_if(
        valid_categories.begin(), valid_categories.end(),
        [&inp_lower](const std::string &category) {
          std::string category_lower = category;
          std::transform(category_lower.begin(), category_lower.end(),
                         category_lower.begin(), [](unsigned char param_char) {
                           return std::tolower(param_char);
                         });
          return category_lower == inp_lower;
        });
    if (itr != valid_categories.end()) {
      m_alert_msg = "Category already exists as \"" + *itr + "\".\n";
      break;
    }

    valid_categories.insert(inp);
  }

    m_state = state::end;
    break;
  case state::end:
  default:
    app.redirect("/");

    // Reset variables for next time user enters the page
    m_state = state::prompt;

    m_alert_msg.clear();
    break;
  }

  return update_action::none;
}
