#include "expense.h"
#include <string>
#include <utility>

expense::expense() : amount(0.0) {}

expense::expense(date_alias date, std::string category, double amount,
                 std::string desc)
    : date(date), category(std::move(category)), amount(amount),
      desc(std::move(desc)) {}

bool expense::date_comparator::operator()(const expense &lhs,
                                          const expense &rhs) const {
  return lhs.date < rhs.date;
}
