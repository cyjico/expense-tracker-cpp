#include "date.h"
#include <cstdint>
#include <sstream>
#include <stdexcept>
#include <string>

date::date() : day(1), month(1), year(0) {}

// Skill issue if you swap these parameters
// NOLINTBEGIN(bugprone-easily-swappable-parameters)
date::date(uint8_t day, uint8_t month, uint16_t year)
    : day(day), month(month), year(year) {}
// NOLINTEND(bugprone-easily-swappable-parameters)

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

bool date::operator==(const date &other) const {
  return year == other.year && month == other.month && day == other.day;
}

bool date::operator!=(const date &other) const { return !(*this == other); }

bool date::operator<(const date &other) const {
  if (year != other.year) {
    return year < other.year;
  }
  if (month != other.month) {
    return month < other.month;
  }
  return day < other.day;
}

bool date::operator>(const date &other) const {
  if (year != other.year) {
    return year > other.year;
  }
  if (month != other.month) {
    return month > other.month;
  }
  return day > other.day;
}

bool date::operator<=(const date &other) const {
  return *this < other || *this == other;
}

bool date::operator>=(const date &other) const {
  return *this > other || *this == other;
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
  return date;
}

std::string date::get_name(const uint8_t &day, const uint8_t &month,
                           const uint16_t &year) {
  if (day < 1 || day > 31) {
    return get_name(month, year);
  }

  return std::string(get_month_name(month)) + " " + std::to_string(day) + ", " +
         std::to_string(year);
}

std::string date::get_name(const uint8_t &month, const uint16_t &year) {
  if (month < 1 || month > 12) {
    return get_name(year);
  }

  return std::string(get_month_name(month)) + " " + std::to_string(year);
}

std::string date::get_name(const uint16_t &year) {
  return std::to_string(year);
}
