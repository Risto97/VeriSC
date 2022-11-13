cmake_minimum_required(VERSION 3.8)
project(common_lib VERSION 1.0)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE
                           "${CMAKE_CURRENT_SOURCE_DIR}/inc")

