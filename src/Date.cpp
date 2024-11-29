#include "date.h"
#include <cstdint>
#include <sstream>
#include <stdexcept>
#include <string>

date::date() : day(1), month(1), year(0) {}

// Why? because, I do not know how to fix it 🤯
// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
date::date(uint8_t day, uint8_t month, uint16_t year)
    : day(day), month(month), year(year) {}

std::string date::to_string() const {
  std::ostringstream oss;
  oss << static_cast<int>(day) << "/" << static_cast<int>(month) << "/" << year;
  return oss.str();
}

bool date::is_valid() const {
  constexpr uint8_t max_days = 31;
  constexpr uint8_t max_months = 12;

  return day >= 1 && day <= max_days && month >= 1 && month <= max_months;
}

bool date::operator<(const date &other) const {
  return year < other.year || month < other.month || day < other.day;
}

bool date::operator>(const date &other) const {
  return year > other.year || month > other.month || day > other.day;
}

date date::from_string(const std::string &date_str) {
  date date;

  std::stringstream stream(date_str);

  std::string day_str;
  std::string month_str;
  std::string year_str;
  if (!std::getline(stream, day_str, '/') ||
      !std::getline(stream, month_str, '/') ||
      !std::getline(stream, year_str)) {
    throw std::runtime_error(R"(Invalid date format: expected "dd/mm/yyyy")");
  }

  date.day = std::stoi(day_str);
  date.month = std::stoi(month_str);
  date.year = std::stoi(year_str);
  if (!date.is_valid()) {
    throw std::runtime_error("Invalid value/s for day, month, or/and year");
  }

  return date;
}
