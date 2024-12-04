#pragma once
#include "date.h"
#include <string>

using date_alias = date;

/**
 * @brief Data representation of an expense.
 */
struct expense {
  date_alias date;
  std::string category;
  double amount;
  std::string desc;

  expense();
  explicit expense(date_alias param_date, std::string param_category,
                   double param_amount, std::string param_desc);

  struct date_comparator {
    bool operator()(const expense &lhs, const expense &rhs) const;
  };
};
