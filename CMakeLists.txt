cmake_minimum_required(VERSION 3.12)

set(PROJECT_NAME example-kde-jobtracker)
set(TARGET_NAME ${PROJECT_NAME})

project(${PROJECT_NAME} VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")

find_package(Qt5 REQUIRED COMPONENTS Core)
find_package(spdlog REQUIRED)

find_package(KF5JobWidgets REQUIRED)
find_package(KF5I18n REQUIRED)

set(CMAKE_AUTOMOC ON)

add_executable(${TARGET_NAME})

target_sources(${TARGET_NAME} PRIVATE
    "source/job.hpp"
    "source/main.cpp"
    "source/worker.hpp")

target_link_libraries(${TARGET_NAME} PUBLIC
    Qt5::Core Qt5::Widgets spdlog::spdlog
    KF5::I18n KF5::JobWidgets
    ${MEDIACOPIER_LIB_TARGET_NAME})

add_custom_command(
    TARGET ${TARGET_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            "${CMAKE_CURRENT_SOURCE_DIR}/test.sh"
            "${CMAKE_CURRENT_BINARY_DIR}/test.sh")
