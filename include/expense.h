#pragma once
#include "date.h"
#include <string>

struct expense {
  date date;
  std::string category;
  float amount;
  std::string desc;

  explicit expense();
  explicit expense(struct date date, std::string category, float amount,
                   std::string desc);

  std::string to_string() const;
};
