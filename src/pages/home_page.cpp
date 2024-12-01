#include "pages/home_page.h"
#include "application.h"
#include "date.h"
#include "events/page_event_emitter.h"
#include "expense.h"
#include "pages/abstract_page.h"
#include "utils/utils.h"
#include <exception>
#include <fstream>
#include <iostream>
#include <ostream>
#include <set>
#include <sstream>
#include <string>
#include <unordered_set>

namespace {

constexpr char delimiter = '\x1E';
constexpr const char *file_name = ".etkcpp_expenses.txt";

} // namespace

home_page::home_page() = default;

void home_page::attach_listeners(application &app) {
  app.onpageload().add_listener([this](page_event evt) -> void {
    if (evt.app->cur_page() != shared_from_this()) {
      return;
    }

    if (!evt.app->has_shared_datum("expenses")) {
      // Initialize the expenses and valid_categories in shared_data
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
          if (std::getline(stream, date_str, delimiter) &&
              std::getline(stream, category, delimiter) &&
              std::getline(stream, amount_str, delimiter)) {

            expense expense;
            expense.date = date::from_string(date_str);
            expense.category = category;
            expense.amount = std::stod(amount_str);

            std::string desc;
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
  utils::clear_screen(cout);

  cout << "1. Add Expense\n"
          "2. View Expenses\n"
          "3. Search Expenses\n"
          "4. Generate Monthly Report\n"
          "5. Save and Exit\n";

  std::string inp;
  std::getline(cin, inp);

  int option = 0;
  try {
    option = std::stoi(utils::trim_string(inp));
  } catch (const std::exception &e) {
    return update_action::none;
  }

  if (option < 1 || option > 5) {
    return update_action::none;
  }

  switch (option) {
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
      out_file << expense.date.to_string() << delimiter << expense.category
               << delimiter << utils::double_to_string(expense.amount, 0, false)
               << delimiter << expense.desc << '\n';
    }

    out_file.close();
  }
    return update_action::exit_app;
  default:
    break;
  }

  return update_action::none;
}
