#include "pages/home_page.h"
#include "abstract_page.h"
#include "application.h"
#include "util.h"
#include <iostream>
#include <limits>

home_page::home_page() = default;

void home_page::render(application & /*app*/, std::ostream &cout) {
  // \x1B      => ASCII escape character in hexadecimal
  // \x1B[2J   => Clear entire screen (J) from top to bottom (2)
  // \x1B[1;1H => Position cursor at 1st row (1) and 1st column (1H)
  cout << "\x1B[2J\x1B[1;1H";
  // For more info, see https://en.wikipedia.org/wiki/ANSI_escape_code

  cout << "1. Add Expense\n"
          "2. View Expenses\n"
          "3. Search Expenses by Category\n"
          "4. Generate Monthly Report\n"
          "5. Save and Exit\n";
}

update_action home_page::update(application &app, std::istream &cin) {
  int inp = 0;
  cin >> inp;
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

  if (!app.has_shared_datum("expense")) {
    app.insert_or_assign_shared_datum("expense", "");
  }

  if (util::clear_failed_istream(cin)) {
    return update_action::render_next_frame;
  }

  constexpr int add_expense = 1;
  constexpr int view_expenses = 2;
  constexpr int search_expenses = 3;
  constexpr int generate_report = 4;
  constexpr int save_and_exit = 5;

  if (inp < add_expense || inp > save_and_exit) {
    return update_action::render_next_frame;
  }

  switch (inp) {
  case add_expense:
    app.redirect("/add-expense");
    break;
  case view_expenses:
    app.redirect("/view-expense");
    break;
  case search_expenses:
    app.redirect("/search-expenses");
    break;
  case generate_report:
    app.redirect("/generate-report");
    break;
  case save_and_exit:
    app.redirect("/save-and-exit");
    break;
  default:
    break;
  }

  return update_action::render_next_frame;
}