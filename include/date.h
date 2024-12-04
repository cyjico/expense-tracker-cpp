#pragma once
#include <cstdint>
#include <stdexcept>
#include <string>

/**
 * @brief Data representation of a date.
 */
struct date {
  /**
   * @brief A day, can be beyond 31 or 0.
   *
   * `uint8_t` was chosen because a day can only reach up to 31 which is 5 bits,
   * nearest being 8 bits.
   */
  uint8_t day;
  /**
   * @brief A month, can be beyond 12 or 0.
   *
   * `uint8_t` was chosen for ease of use.
   */
  uint8_t month;
  /**
   * @brief A year, can be 0.
   *
   * `uint16_t` was chosen because a year is usually repsented as "yyyy".
   */
  uint16_t year;

  date();
  date(uint8_t param_day, uint8_t param_month, uint16_t param_year);

  std::string to_string() const;
  bool is_valid() const;

  bool operator==(const date &other) const;
  bool operator!=(const date &other) const;
  bool operator<(const date &other) const;
  bool operator>(const date &other) const;
  bool operator>=(const date &other) const;
  bool operator<=(const date &other) const;

  /**
   * @brief Creates a date from "dd/mm/yyyy".
   *
   * It does not check if the values are within logical range.
   *
   * @param date_str String to convert into a date instance.
   * @return Date instanced created from the string.
   * @throws std::runtime_error if it does not follow "dd/mm/yyyy".
   * std::runtime_error if the "dd" or "mm" exceeds 255 or under 0.
   * std::runtime_error if "yyyy" exceeds 65535 or under 0.
   */
  static date from_string(const std::string &date_str);

  static std::string get_name(const uint8_t &day, const uint8_t &month,
                              const uint16_t &year);
  static std::string get_name(const uint8_t &month, const uint16_t &year);
  static std::string get_name(const uint16_t &year);

  /**
   * @brief Retrieves the name of the month from the month number.
   *
   * @param month Month number from 1 to 12.
   * @return Name of the month.
   * @throws std::out_of_range if the month is not between 1 and 12.
   */
  static constexpr const char *get_month_name(uint8_t month);
};

constexpr const char *date::get_month_name(uint8_t month) {
  switch (month) {
  case 1:
    return "January";
  case 2:
    return "February";
  case 3:
    return "March";
  case 4:
    return "April";
  case 5:
    return "May";
  case 6:
    return "June";
  case 7:
    return "July";
  case 8:
    return "August";
  case 9:
    return "September";
  case 10:
    return "October";
  case 11:
    return "November";
  case 12:
    return "December";
  default:
    throw std::out_of_range("Month must be between 1 and 12");
  }
}
