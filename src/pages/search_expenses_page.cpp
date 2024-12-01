#include "pages/search_expenses_page.h"
#include "application.h"
#include "pages/abstract_page.h"
#include "utils/utils.h"
#include <exception>
#include <iostream>
#include <string>

search_expenses_page::search_expenses_page() = default;

update_action search_expenses_page::update(application &app, std::ostream &cout,
                                           std::istream &cin) {
  utils::clear_screen(cout);
  cout << "1. Find expenses by category\n"
          "2. Find expenses by datetime (day/month/year)\n"
          "3. Exit\n";

  std::string inp;
  std::getline(cin, inp);

  int option = 0;
  try {
    option = std::stoi(utils::trim_string(inp));
  } catch (const std::exception &e) {
    return update_action::none;
  }

  switch (option) {
  case 1:
    app.redirect("/search-expenses/by-category");
    break;
  case 2:
    app.redirect("/search-expenses/by-datetime");
    break;
  case 3:
    app.redirect("/");
    break;
  default:
    break;
  }

  return update_action::none;
}
