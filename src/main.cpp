#include "application.h"
#include <cstdlib>
#include <exception>
#include <fstream>
#include <iostream>

namespace {

[[noreturn]]
void main_terminate_handler() noexcept {
  std::cerr << "Unhandled exception! Terminating program." << '\n';

  // Output exception into an error.log file.
  try {
    const std::exception_ptr except = std::current_exception();
    if (except) {
      std::rethrow_exception(except);
    }
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << '\n';

    std::ofstream log_file("error.log", std::ios::app);
    if (log_file) {
      log_file << "Error: " << e.what() << '\n';
    }
  } catch (...) {
    std::cerr << "Error: Unknown exception type" << '\n';

    std::ofstream log_file("error.log", std::ios::app);
    if (log_file) {
      log_file << "Error: Unknown exception type\n";
    }
  }

  std::abort();
}

} // namespace

// NOLINTNEXTLINE(bugprone-exception-escape)
int main() {
  std::set_terminate(main_terminate_handler);

  application app;
  app.run_indefinitely();

  std::cout << "Reached end of program.";
  return 0;
}
