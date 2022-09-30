cmake_minimum_required(VERSION 3.8)
project(rtl)

find_package(verilator HINTS $ENV{VERILATOR_ROOT} ${VERILATOR_ROOT})
if (NOT verilator_FOUND)
  message(FATAL_ERROR "Verilator was not found. Either install it, or set the VERILATOR_ROOT environment variable")
endif()

set(CMAKE_VERBOSE_MAKEFILE ON)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(USE_VERILATOR True)


# SystemC dependencies
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# Find SystemC using SystemC's CMake integration
set(CMAKE_PREFIX_PATH "${SYSTEMC}" "${SYSTEMC}/build")
find_package(SystemCLanguage CONFIG REQUIRED)
if (NOT SystemCLanguage_FOUND)
    message(FATAL_ERROR "SystemC not found")
endif()

set (CMAKE_CXX_STANDARD ${SystemC_CXX_STANDARD} CACHE STRING
     "C++ standard to build all targets. Supported values are 98, 11, and 14.")
set (CMAKE_CXX_STANDARD_REQUIRED ${SystemC_CXX_STANDARD_REQUIRED} CACHE BOOL
     "The with CMAKE_CXX_STANDARD selected C++ standard is a requirement.")

add_library(${PROJECT_NAME})

# set_property(
#     TARGET ${PROJECT_NAME}
#   PROPERTY CXX_STANDARD ${SystemC_CXX_STANDARD}
#   )

# Add the Verilated circuit to the target
verilate(${PROJECT_NAME} SYSTEMC
        SOURCES nmi2apb.v
        VERILATOR_ARGS --pins-sc-uint --trace -no-trace-params --trace-structs
        )

verilator_link_systemc(${PROJECT_NAME})
