#pragma once
#include "date.h"
#include <string>

/**
 * @brief Data representation of an expense.
 */
struct expense {
  using date_alias = struct date;

  date_alias date;
  std::string category;
  double amount;
  std::string desc;

  expense();
  explicit expense(date_alias date, std::string category, double amount,
                   std::string desc);

  struct date_comparator {
    bool operator()(const expense &lhs, const expense &rhs) const;
  };
};
