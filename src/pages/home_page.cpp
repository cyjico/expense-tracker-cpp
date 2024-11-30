#include "pages/home_page.h"
#include "application.h"
#include "date.h"
#include "events/page_event_emitter.h"
#include "expense.h"
#include "pages/abstract_page.h"
#include "utils/utils.h"
#include <fstream>
#include <iostream>
#include <limits>
#include <ostream>
#include <set>
#include <sstream>
#include <string>
#include <unordered_set>

static constexpr const char *file_name = ".etkcpp_expenses.txt";

home_page::home_page() = default;

void home_page::attach_listeners(application &app) {
  app.onpageload().add_listener([this](page_event evt) -> void {
    if (evt.app->cur_page() != shared_from_this()) {
      return;
    }

    if (!evt.app->has_shared_datum("expenses")) {
      auto expenses = std::multiset<expense>();
      auto valid_categories = std::unordered_set<std::string>(
          {"Food", "Transportation", "Rent", "Miscellaneous"});

      std::ifstream input_file(file_name);
      if (input_file.is_open()) {
        std::string line;
        while (std::getline(input_file, line)) {
          std::stringstream stream(line);

          std::string date_str;
          std::string category;
          std::string amount_str;
          std::string desc;
          if (std::getline(stream, date_str, ',') &&
              std::getline(stream, category, ',') &&
              std::getline(stream, amount_str, ',')) {

            expense expense;
            expense.date = date::from_string(date_str);
            expense.category = category;
            expense.amount = std::stod(amount_str);
            expense.desc = std::getline(stream, desc) ? desc : "";

            expenses.insert(expense);
            valid_categories.insert(expense.category);
          }
        }

        input_file.close();
      }

      evt.app->insert_or_assign_shared_datum("expenses", expenses);
      evt.app->insert_or_assign_shared_datum("valid_categories",
                                             valid_categories);
    }
  });
}

update_action home_page::update(application &app, std::ostream &cout,
                                std::istream &cin) {
  // \x1B      => ASCII escape character in hexadecimal
  // \x1B[2J   => Clear entire screen (J) from top to bottom (2)
  // \x1B[1;1H => Position cursor at 1st row (1) and 1st column (1H)
  cout << "\x1B[2J\x1B[1;1H" << std::flush;
  // For more info, see https://en.wikipedia.org/wiki/ANSI_escape_code

  cout << "1. Add Expense\n"
          "2. View Expenses\n"
          "3. Search Expenses\n"
          "4. Generate Monthly Report\n"
          "5. Save and Exit\n";

  int inp = 0;
  cin >> inp;
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

  if (utils::clear_failed_istream(cin)) {
    return update_action::none;
  }

  if (inp < 1 || inp > 5) {
    return update_action::none;
  }

  switch (inp) {
  case 1:
    app.redirect("/add-expense");
    break;
  case 2:
    app.redirect("/view-expenses");
    break;
  case 3:
    app.redirect("/search-expenses");
    break;
  case 4:
    app.redirect("/generate-report");
    break;
  case 5: {
    std::ofstream out_file(file_name);
    if (!out_file.is_open()) {
      break;
    }

    const auto &expenses =
        app.at_shared_datum<std::multiset<expense>>("expenses");
    for (const auto &expense : expenses) {
      out_file << expense.to_string() << "\n";
    }

    out_file.close();
  }
    return update_action::exit_app;
  default:
    break;
  }

  return update_action::none;
}
