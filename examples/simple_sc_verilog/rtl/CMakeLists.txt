cmake_minimum_required(VERSION 3.8)
project(adder_rtl)

add_library(${PROJECT_NAME} INTERFACE)

set_property(TARGET ${PROJECT_NAME}
    PROPERTY INTERFACE_V_SOURCES
    ${PROJECT_SOURCE_DIR}/adder.v
    )
