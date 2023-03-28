cmake_minimum_required(VERSION 3.20)

set(LIB_TARGET_NAME LibCryptoHistory)

set(SOURCE_FILES "libcryptohistory/database.cpp")

add_library(${LIB_TARGET_NAME} ${SOURCE_FILES})

target_include_directories(${LIB_TARGET_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/lib)

target_link_libraries(
  ${LIB_TARGET_NAME}
  PUBLIC unqlitepp::unqlitepp
  PRIVATE project_options)

