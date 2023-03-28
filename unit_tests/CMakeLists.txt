cmake_minimum_required(VERSION 3.20)

set(EXE_TARGET_NAME unit_tests_LibCryptoHistory)

find_package(Catch2 REQUIRED)

add_executable(${EXE_TARGET_NAME} "test_database.cpp" "test_trade.cpp")

target_link_libraries(${EXE_TARGET_NAME} PRIVATE Catch2::Catch2WithMain LibCryptoHistory project_options)

include(Catch)
catch_discover_tests(${EXE_TARGET_NAME})
