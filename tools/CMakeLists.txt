find_package(gcc CONFIG REQUIRED
    PATHS ${VERISC_INSTALL_DIR}/*
    NO_DEFAULT_PATH
    )
include(${CMAKE_CURRENT_LIST_DIR}/../verisc_config.cmake)
if(NOT VERISC_CFG STREQUAL "")
    include(${VERISC_CFG})
endif()

cmake_minimum_required(VERSION 3.20)
project(tools_install)

include(ExternalProject)

if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
		cmake_policy(SET CMP0135 NEW)
endif()

set(GROUP_INSTALL_DIR ${CMAKE_INSTALL_PREFIX})


set(PO4A_VERSION 0.69)
set(HELP2MAN_VERSION 1.49.3)

set(ICSC_COMPILER_INSTALL_DIR ${GROUP_INSTALL_DIR}/systemc-compiler-${ICSC_COMPILER_VERSION})

if((DEFINED SYSTEMC_HOME))
    set(SYSTEMC_INSTALL_DIR ${SYSTEMC_HOME})
else()
    set(SYSTEMC_INSTALL_DIR ${GROUP_INSTALL_DIR}/systemc-${SYSTEMC_VERSION}) # This one is used by uvm and scv so its here
endif()

set(VERILATOR_INSTALL_DIR ${GROUP_INSTALL_DIR}/verilator-${VERILATOR_VERSION})

add_subdirectory(systemc)

if(NOT UVM-SYSTEMC_VERSION STREQUAL "FALSE")
    add_subdirectory(uvm-systemc)
endif()
if(NOT SCV_VERSION STREQUAL "FALSE")
    add_subdirectory(scv)
endif()
if(NOT FC4SC_VERSION STREQUAL "FALSE")
    add_subdirectory(fc4sc)
endif()

add_subdirectory(verilator)



install(
    FILES "${PROJECT_SOURCE_DIR}/dummy_file_dont_delete"
    DESTINATION ${CMAKE_INSTALL_PREFIX}
    RENAME dummy_file_you_can_delete
    )
