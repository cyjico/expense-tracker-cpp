#pragma once
#include <cstdint>
#include <stdexcept>
#include <string>

struct date {
  uint8_t day;
  uint8_t month;
  uint16_t year;

  date();
  date(uint8_t day, uint8_t month, uint16_t year);

  std::string to_string() const;
  bool is_valid() const;

  /**
   * @brief Creates a date from "dd/mm/yyyy".
   *
   * @param date_str
   * @return
   */
  static date from_string(const std::string &date_str);

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
