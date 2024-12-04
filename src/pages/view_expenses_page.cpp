#include "pages/view_expenses_page.h"
#include "application.h"
#include "events/page_event_emitter.h"
#include "expense.h"
#include "pages/abstract_page.h"
#include "utils/utils.h"
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <iostream>
#include <ostream>
#include <set>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

void view_expenses_page::render_cell(std::ostream &cout,
                                     const std::string &text) const {
  const auto times = m_table_cell_width - text.length() + m_table_cell_padding;
  cout << text << std::string(times, ' ');
}

void view_expenses_page::render_horizontal_rule(std::ostream &cout,
                                                uint64_t cells) const {
  cout << std::string(static_cast<size_t>(
                          (m_table_cell_width + m_table_cell_padding) * cells),
                      '-')
       << '\n';
}

void view_expenses_page::sort_rows(const state &state,
                                   std::vector<expense> &table_rows) {
  switch (state) {
  case state::show_sort_by_date:
    break;
  case state::show_sort_by_amount:
    std::sort(table_rows.begin(), table_rows.end(),
              [](const expense &lhs, const expense &rhs) -> bool {
                return lhs.amount < rhs.amount;
              });
    break;
  case state::show_sort_by_category:
    std::sort(table_rows.begin(), table_rows.end(),
              [](const expense &lhs, const expense &rhs) -> bool {
                return lhs.category < rhs.category;
              });
    break;
  case state::end:
  default:
    throw std::runtime_error("Cannot sort table_rows when state is state::end");
  }
}

// --- public ---

view_expenses_page::view_expenses_page(uint32_t table_cell_padding)
    : m_table_cell_padding(table_cell_padding) {}

constexpr uint32_t default_table_cell_width = 15;

void view_expenses_page::attach_listeners(application &app) {
  app.onpageload().add_listener([this](page_event evt) -> void {
    if (evt.app->cur_page() != shared_from_this()) {
      return;
    }

    /*
     * Why no checking?
     * We expect the app to always arrive FIRST at `home_page`.
     * Then, as it follows, we SHOULD throw an error if the page was
     * unexpectedly loaded before `home_page`.
     */
    const auto &expenses =
        evt.app->at_shared_datum<application::expense_datum>("expenses");

    // Retrieve maximum cell width
    m_table_cell_width = default_table_cell_width;
    for (const auto &expense : expenses) {
      m_table_cell_width = static_cast<uint32_t>(std::max(
          {expense.date.to_string().length(), expense.category.length(),
           utils::double_to_string(expense.amount).length(),
           expense.desc.length(), static_cast<size_t>(m_table_cell_width)}));
    }

    std::swap(m_prev_state, m_state);

    m_table_rows = std::vector<expense>(expenses.begin(), expenses.end());
    sort_rows(m_state, m_table_rows);
  });
}

update_action view_expenses_page::update(application &app, std::ostream &cout,
                                         std::istream &cin) {
  utils::clear_screen(cout);

  // Render table
  render_row<std::string, std::string, std::string, std::string>(
      cout, "Date", "Category", "Amount", "Description");
  render_horizontal_rule(cout);

  double total_expenses = 0;
  for (const auto &expense : m_table_rows) {
    total_expenses += expense.amount;

    render_cell(cout, expense.date.to_string());
    render_cell(cout, expense.category);
    render_cell(cout, utils::double_to_string(expense.amount));
    render_cell(cout, expense.desc);

    cout << '\n';
  }

  render_horizontal_rule(cout);
  render_cell(cout, "Total Expenses:");
  render_cell(cout, utils::double_to_string(total_expenses));

  // Render sorting options and exit option
  cout << "\n\n";

  constexpr std::array<std::pair<state, const char *>, 3> sort_options = {
      {{state::show_sort_by_date, "Sort by Date"},
       {state::show_sort_by_category, "Sort by Category"},
       {state::show_sort_by_amount, "Sort by Amount"}}};

  size_t item_number = 1;
  for (const auto &[option_state, label] : sort_options) {
    cout << item_number++ << ". " << label
         << (m_state == option_state ? " (selected)\n" : "\n");
  }

  cout << item_number << ". Exit\n";

  std::string inp;
  std::getline(cin, inp);

  int desired_state = 0;
  try {
    desired_state = std::stoi(inp);
  } catch (const std::exception &) {
    return update_action::none;
  }

  if (desired_state < 0 || desired_state > 4 || m_state == m_prev_state) {
    return update_action::none;
  }

  m_prev_state = m_state;
  m_state = static_cast<state>(desired_state);

  const auto &expenses =
      app.at_shared_datum<application::expense_datum>("expenses");

  m_table_rows = std::vector<expense>(expenses.begin(), expenses.end());

  switch (m_state) {
  case state::show_sort_by_date:
  case state::show_sort_by_amount:
  case state::show_sort_by_category:
    // Why std::vector?
    // std::sort requires containers that support random access iterators
    sort_rows(m_state, m_table_rows);
    break;
  case state::end:
  default:
    app.redirect("/");
    break;
  }

  return update_action::none;
}
