cmake_minimum_required(VERSION 3.8)
project(nmi2apb_rtl)

list(APPEND V_SOURCES
    ${PROJECT_SOURCE_DIR}/nmi2apb.v
    )

add_library(${PROJECT_NAME} INTERFACE)

set_property(TARGET ${PROJECT_NAME}
            PROPERTY INTERFACE_V_SOURCES
            ${V_SOURCES}
            )
