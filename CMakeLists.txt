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
set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

file(GLOB_RECURSE SRC_FILES ${SRC_DIR}/*.cpp)
file(GLOB_RECURSE INCLUDE_FILES ${INCLUDE_DIR}/*.h)

include_directories(${INCLUDE_DIR})
add_executable(${PROJECT_NAME} ${SRC_FILES})

target_compile_options(${PROJECT_NAME} PRIVATE -Wall)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Setup clang-format
find_program(CLANG_FORMAT_BIN clang-format)
if(CLANG_FORMAT_BIN)
  add_custom_target(
    clang-format
    COMMAND ${CLANG_FORMAT_BIN} -i -style=file ${SRC_FILES} ${INCLUDE_FILES}
    COMMENT "Running clang-format on source and include files")
else()
  message(WARNING "clang-format not found. Skipping format.")
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
  message(WARNING "cppcheck not found. Skipping static analysis.")
endif()

add_dependencies(${PROJECT_NAME} cppcheck clang-format)
