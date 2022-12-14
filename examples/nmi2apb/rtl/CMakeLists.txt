cmake_minimum_required(VERSION 3.8)
project(nmi2apb_rtl)

add_library(${PROJECT_NAME} INTERFACE
    ${PROJECT_SOURCE_DIR}/nmi2apb.v
    )
