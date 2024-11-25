cmake_minimum_required(VERSION 3.10)

project(
  expense-tracker-cpp
  VERSION 0.1.0
  DESCRIPTION "Final project for CS 1131."
  HOMEPAGE_URL "https://github.com/cyjico/expense-tracker-cpp"
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)
file(GLOB_RECURSE SRC_FILES ${SRC_DIR}/*.cpp ${SRC_DIR}/*.h)

add_executable(${PROJECT_NAME} ${SRC_FILES})

# Setup clang-format and cppcheck
find_program(CLANG_FORMAT clang-format)
find_program(CPPCHECK cppcheck)

if(CLANG_FORMAT OR CPPCHECK)
  # Run clang-format if available
  if(CLANG_FORMAT)
    add_custom_command(
      TARGET ${PROJECT_NAME}
      PRE_BUILD
      COMMAND ${CLANG_FORMAT} -i -style=file ${SRC_FILES}
      COMMENT "Running clang-format on source files")
  else()
    message(WARNING "clang-format not found. Skipping format.")
  endif()

  # Run cppcheck if available
  if(CPPCHECK)
    add_custom_command(
      TARGET ${PROJECT_NAME}
      PRE_BUILD
      COMMAND ${CPPCHECK} --enable=all --std=c++17
              --suppress=missingIncludeSystem ${SRC_FILES}
      COMMENT "Running cppcheck on source files")
  else()
    message(WARNING "cppcheck not found. Skipping static analysis.")
  endif()
endif()
