#pragma once
#include "date.h"
#include <string>

struct expense {
  date date;
  std::string category;
  double amount;
  std::string desc;

  expense();
  explicit expense(struct date date, std::string category, double amount,
                   std::string desc);

  std::string to_string() const;

  bool operator<(const expense &other) const;
  bool operator>(const expense &other) const;
};
