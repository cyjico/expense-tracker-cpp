#include "application.h"
#include "utils/utils.h"
#include <cstdlib>
#include <exception>
#include <fstream>
#include <iostream>

namespace {

[[noreturn]]
void main_terminate_handler() noexcept {
  std::cerr << "Unhandled exception! Terminating program." << '\n';

  // Output exception into an .log file.
  const std::exception_ptr except = std::current_exception();
  if (except) {
    try {
      std::rethrow_exception(except);
    } catch (const std::exception &e) {
      std::ofstream log_file("etkcpp_error.log", std::ios::app);
      if (log_file) {
        log_file << "Error: " << e.what() << '\n';
      }
    } catch (...) {
      std::ofstream log_file("etkcpp_error.log", std::ios::app);
      if (log_file) {
        log_file << "Error: Unknown exception type\n";
      }
    }
  }

  std::abort();
}

} // namespace

int main() {
  std::set_terminate(main_terminate_handler);

  application app;
  app.run_indefinitely();

  utils::clear_screen(std::cout);
  std::cout << "Reached end of program.\n\n";
  return 0;
}
