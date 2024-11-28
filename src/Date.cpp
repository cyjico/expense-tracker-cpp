#include "Date.h"
#include <cstdint>
#include <sstream>
#include <string>

std::string Date::ToString() const {
  std::ostringstream oss;
  oss << static_cast<int>(day) << "/" << static_cast<int>(month) << "/" << year;
  return oss.str();
}

bool Date::IsValid() const {
  constexpr uint8_t max_days = 31;
  constexpr uint8_t max_months = 12;

  return day >= 1 && day <= max_days && month >= 1 && month <= max_months;
}

// Why? because, I do not know how to fix it 🤯
// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
Date::Date(uint8_t day, uint8_t month, uint16_t year)
    : day(day), month(month), year(year) {}
