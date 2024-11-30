#include "pages/view_expense_page.h"
#include "application.h"
#include "events/page_event_emitter.h"
#include "expense.h"
#include "pages/abstract_page.h"
#include "utils/utils.h"
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <ostream>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

void view_expense_page::render_cell(std::ostream &cout,
                                    const std::string &text) const {
  const auto times = m_table_cell_width - text.length() + m_table_cell_padding;
  cout << text << std::string(times, ' ');
}

void view_expense_page::render_horizontal_rule(std::ostream &cout) const {
  cout << std::string(static_cast<size_t>(
                          (m_table_cell_width + m_table_cell_padding) * 4),
                      '-')
       << "\n";
}

view_expense_page::view_expense_page(uint32_t table_cell_padding)
    : m_table_cell_width(15), m_table_cell_padding(table_cell_padding) {}

void view_expense_page::attach_listeners(application &app) {
  app.onpageload().add_listener([this](page_event evt) -> void {
    if (evt.app->cur_page() != shared_from_this()) {
      return;
    }

    if (evt.app->has_shared_datum("expenses")) {
      m_table_cell_width = 15;

      auto expenses =
          evt.app->at_shared_datum<std::multiset<struct expense>>("expenses");

      for (const auto &expense : expenses) {
        m_table_cell_width = std::max<uint64_t>(
            {expense.date.to_string().length(), expense.category.length(),
             utils::double_to_string(expense.amount).length(),
             expense.desc.length(), static_cast<uint64_t>(m_table_cell_width)});
      }
    }
  });
}

void view_expense_page::render(application &app, std::ostream &cout) {
  if (m_prev_state == m_state) {
    cout << "\033[2K\r\033[1A\033[2K\r" << std::flush;
    return;
  }

  cout << "\x1B[2J\x1B[1;1H" << std::flush;

  render_row<std::string, std::string, std::string, std::string>(
      cout, "Date", "Category", "Amount", "Description");
  render_horizontal_rule(cout);

  auto expenses =
      app.at_shared_datum<std::multiset<struct expense>>("expenses");
  std::vector<expense> expenses_vec(expenses.begin(), expenses.end());

  switch (m_state) {
  case state::show_sort_by_date:
    break;
  case state::show_sort_by_amount:
    // std::sort requires containers that support random access iterators
    std::sort(expenses_vec.begin(), expenses_vec.end(),
              [](const expense &lhs, const expense &rhs) -> bool {
                return lhs.amount < rhs.amount;
              });
    break;
  case state::show_sort_by_category:
    // std::sort requires containers that support random access iterators
    std::sort(expenses_vec.begin(), expenses_vec.end(),
              [](const expense &lhs, const expense &rhs) -> bool {
                return lhs.category < rhs.category;
              });
    break;
  default:
    break;
  }
  double total_expenses = 0;

  for (const auto &expense : expenses_vec) {
    total_expenses += expense.amount;

    render_cell(cout, expense.date.to_string());
    render_cell(cout, expense.category);
    render_cell(cout, utils::double_to_string(expense.amount));
    render_cell(cout, expense.desc);

    cout << "\n";
  }

  render_horizontal_rule(cout);
  render_cell(cout, "Total Expenses:");
  render_cell(cout, utils::double_to_string(total_expenses));

  cout << "\n\n1. Sort by Date"
       << (m_state == state::show_sort_by_date ? " (selected)\n" : "\n")
       << "2. Sort by Amount"
       << (m_state == state::show_sort_by_amount ? " (selected)\n" : "\n")
       << "3. Sort by Category"
       << (m_state == state::show_sort_by_category ? " (selected)\n" : "\n")
       << "4. Exit\n";
}

update_action view_expense_page::update(application &app, std::istream &cin) {
  std::string inp;
  std::getline(cin, inp);

  int desired_state = 0;

  try {
    desired_state = std::stoi(inp);
  } catch (const std::invalid_argument& e) {
    desired_state = 0;
  }

  m_prev_state = m_state;
  switch (desired_state) {
  case 1:
    m_state = state::show_sort_by_date;
    break;
  case 2:
    m_state = state::show_sort_by_amount;
    break;
  case 3:
    m_state = state::show_sort_by_category;
    break;
  case 4:
    app.redirect("/");

    m_state = state::show_sort_by_date;
    return update_action::render_next_frame;
  default:
    break;
  }

  return update_action::render_next_frame;
}
