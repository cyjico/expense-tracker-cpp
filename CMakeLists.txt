cmake_minimum_required(VERSION 3.10)

project(
  expense-tracker-cpp
  VERSION 0.1.0
  DESCRIPTION "Final project for CS 1131."
  HOMEPAGE_URL "https://github.com/cyjico/expense-tracker-cpp"
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(INCLUDE_FILES
    ${INCLUDE_DIR}/pages/add_expense_page.h
    ${INCLUDE_DIR}/pages/home_page.h
    ${INCLUDE_DIR}/abstract_page.h
    ${INCLUDE_DIR}/application.h
    ${INCLUDE_DIR}/date.h
    ${INCLUDE_DIR}/util.h)

include_directories(${INCLUDE_DIR})

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(SRC_FILES
    ${SRC_DIR}/pages/add_expense_page.cpp ${SRC_DIR}/pages/home_page.cpp
    ${SRC_DIR}/application.cpp ${SRC_DIR}/date.cpp ${SRC_DIR}/main.cpp
    ${SRC_DIR}/util.cpp)

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_compile_options(${PROJECT_NAME} PRIVATE -Wall)

# Setup clang-format
find_program(CLANG_FORMAT_BIN clang-format)
if(CLANG_FORMAT_BIN)
  add_custom_target(
    clang-format
    COMMAND ${CLANG_FORMAT_BIN} -i -style=file ${SRC_FILES} ${INCLUDE_FILES}
    COMMENT "Running clang-format on source and include files")
else()
  message(WARNING "clang-format not found. Skipping formatting.")
endif()

# Setup cppcheck
find_program(CPPCHECK_BIN cppcheck)
if(CPPCHECK_BIN)
  add_custom_target(
    cppcheck
    COMMAND
      ${CPPCHECK_BIN} --enable=all --std=c++17 --language=c++
      --suppress=missingIncludeSystem -I${INCLUDE_DIR} ${SRC_FILES}
      ${INCLUDE_FILES}
    COMMENT "Running cppcheck on source and include files")
else()
  message(WARNING "cppcheck not found. Skipping cppcheck.")
endif()

# Setup clang-tidy
find_program(CLANG_TIDY_BIN clang-tidy)
if(CLANG_TIDY_BIN)
  set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY_BIN})

  add_custom_target(
    clang-tidy-check
    COMMAND ${CLANG_TIDY_BIN} -p=${CMAKE_BINARY_DIR} ${SRC_FILES}
            ${INCLUDE_FILES}
    COMMENT "Running clang-tidy on source and include files")

  add_custom_target(
    clang-tidy-fix
    COMMAND ${CLANG_TIDY_BIN} --fix -p=${CMAKE_BINARY_DIR} ${SRC_FILES}
            ${INCLUDE_FILES}
    COMMENT "Running clang-tidy with --fix on source and include files")
else()
  message(WARNING "clang-tidy not found. Skipping clang-tidy.")
endif()

add_custom_target(
  all-utility
  DEPENDS clang-format cppcheck clang-tidy-check
  COMMENT "Running clang-format, cppcheck, and clang-tidy-check together.")
