#pragma once
#include "pages/abstract_page.h"
#include <cstdint>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <utility>

class view_expense_page
    : public abstract_page,
      public std::enable_shared_from_this<view_expense_page> {
  uint32_t m_table_cell_width;
  uint32_t m_table_cell_padding;

  void render_cell(std::ostream &cout, const std::string &text) const;

  template <typename... Args>
  void render_row(std::ostream &cout, const Args &&...args) const;

public:
  explicit view_expense_page(uint32_t table_cell_padding);

  void attach_listeners(application &app);
  void render(application &app, std::ostream &cout) override;
  update_action update(application &app, std::istream &cin) override;
};

template <typename... Args>
void view_expense_page::render_row(std::ostream &cout,
                                   const Args &&...args) const {
  static_assert((std::is_same_v<Args, std::string> && ...),
                "All arguments must be of type std::string");

  (render_cell(cout, std::forward<const Args>(args)), ...);
  cout << "\n";
}
