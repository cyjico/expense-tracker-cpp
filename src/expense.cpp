#include "expense.h"
#include "date.h"
#include <sstream>
#include <string>
#include <utility>

expense::expense() : date(0, 0, 0), amount(0.0) {}

expense::expense(struct date date, std::string category, double amount,
                 std::string desc)
    : date(date), category(std::move(category)), amount(amount),
      desc(std::move(desc)) {}

bool expense::operator<(const expense &other) const {
  return date < other.date;
}

bool expense::operator>(const expense &other) const {
  return date > other.date;
}

std::string expense::to_string() const {
  std::ostringstream oss;
  oss << date.to_string() << "," << category << "," << amount << "," << desc;
  return oss.str();
}
