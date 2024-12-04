#include "date.h"
#include <cstdint>
#include <sstream>
#include <stdexcept>
#include <string>

date::date() : day(1), month(1), year(0) {}

// Skill issue if you swap these parameters
// NOLINTBEGIN(bugprone-easily-swappable-parameters)
date::date(uint8_t param_day, uint8_t param_month, uint16_t param_year)
    : day(param_day), month(param_month), year(param_year) {}
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

  const int day = std::stoi(day_str);
  const int month = std::stoi(month_str);
  const int year = std::stoi(year_str);

  if (day < 0 || day > 255 || month < 0 || month > 255 || year < 0 ||
      year > 65535) {
    throw std::runtime_error("Invalid date values.");
  }

  date.day = static_cast<uint8_t>(day);
  date.month = static_cast<uint8_t>(month);
  date.year = static_cast<uint8_t>(year);
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
