cmake_minimum_required(VERSION 3.20)
project(QCryptoHistory CXX)

set(EXE_TARGET_NAME QCryptoHistory)

find_package(Qt6 6.4 COMPONENTS Core REQUIRED)

add_executable(${EXE_TARGET_NAME} "main.cpp" "mainwindow.cpp" "header.cpp" "fulltransactionlist.cpp" "fulltransactionmodel.cpp")

set_target_properties(${EXE_TARGET_NAME} PROPERTIES AUTOMOC ON)

target_link_libraries(${EXE_TARGET_NAME} PRIVATE Qt6::Gui Qt6::Widgets LibCryptoHistory project_options)
