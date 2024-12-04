#include "expense.h"
#include <string>
#include <utility>

expense::expense() : amount(0.0) {}

expense::expense(date_alias param_date, std::string param_category,
                 double param_amount, std::string param_desc)
    : date(param_date), category(std::move(param_category)),
      amount(param_amount), desc(std::move(param_desc)) {}

bool expense::date_comparator::operator()(const expense &lhs,
                                          const expense &rhs) const {
  return lhs.date < rhs.date;
}
