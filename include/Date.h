#pragma once
#include <cstdint>
#include <string>

struct date {
  uint8_t day;
  uint8_t month;
  uint16_t year;

  date();
  date(uint8_t day, uint8_t month, uint16_t year);

  std::string to_string() const;
  bool is_valid() const;
};
