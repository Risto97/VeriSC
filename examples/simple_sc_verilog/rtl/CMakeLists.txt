cmake_minimum_required(VERSION 3.8)
project(adder_rtl)

add_library(${PROJECT_NAME} INTERFACE
    ${PROJECT_SOURCE_DIR}/adder.v
    )
