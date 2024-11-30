#pragma once
#include "expense.h"
#include "pages/abstract_page.h"
#include <cstdint>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

class view_expenses_page
    : public abstract_page,
      public std::enable_shared_from_this<view_expenses_page> {
  enum class state : std::uint8_t {
    show_sort_by_date = 1,
    show_sort_by_category,
    show_sort_by_amount,
    end,
  };
  state m_prev_state = state::show_sort_by_date;
  state m_state = state::end;

  std::vector<expense> m_table_rows;
  uint32_t m_table_cell_width = 0;
  uint32_t m_table_cell_padding;

  static void sort_rows(const state &state, std::vector<expense> &table_rows);

  void render_cell(std::ostream &cout, const std::string &text) const;
  template <typename... Args>
  void render_row(std::ostream &cout, const Args &&...args) const;
  void render_horizontal_rule(std::ostream &cout) const;

public:
  explicit view_expenses_page(uint32_t table_cell_padding);

  void attach_listeners(application &app);
  update_action update(application &app, std::ostream &cout,
                       std::istream &cin) override;
};

template <typename... Args>
void view_expenses_page::render_row(std::ostream &cout,
                                    const Args &&...args) const {
  static_assert((std::is_same_v<Args, std::string> && ...),
                "All arguments must be of type std::string");

  (render_cell(cout, std::forward<const Args>(args)), ...);
  cout << "\n";
}
