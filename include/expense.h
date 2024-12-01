#pragma once
#include "date.h"
#include <string>

/**
 * @brief Data representation of an expense.
 */
struct expense {
  date date;
  std::string category;
  double amount;
  std::string desc;

  expense();
  explicit expense(struct date date, std::string category, double amount,
                   std::string desc);

  bool operator<(const expense &other) const;
  bool operator>(const expense &other) const;
};
