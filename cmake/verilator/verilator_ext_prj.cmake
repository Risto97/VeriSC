cmake_minimum_required(VERSION 3.12)
project(verilate_ext_prj)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

find_package(SystemCLanguage CONFIG REQUIRED
    PATHS $ENV{VERISC_HOME}/open/*
    NO_DEFAULT_PATH
    )

find_package(verilator HINTS ${VERILATOR_ROOT})
if (NOT verilator_FOUND)
  message(FATAL_ERROR "Verilator was not found. Either install it, or set the VERILATOR_ROOT environment variable")
endif()

foreach(def ${VERILOG_DEFS})
    list(APPEND VLT_DEFS -D${def})
endforeach()

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

string(REPLACE " " ";" VLT_ARGS "${VLT_ARGS}")
list(APPEND ARGS
    --pins-sc-uint
    --trace
    --trace-structs
     -Wno-fatal
     ${VLT_ARGS}
    )

add_library(${TOP_MODULE})
verilate(${TOP_MODULE} SYSTEMC
    SOURCES ${V_SOURCES} ${VLT_CFG_FILES}
    TOP_MODULE ${TOP_MODULE}
    PREFIX ${TOP_MODULE}
    DIRECTORY ${PROJECT_BINARY_DIR}/${TOP_MODULE}_vlt
    INCLUDE_DIRS ${INCLUDE_DIRS}
    VERILATOR_ARGS ${ARGS} ${VLT_DEFS}
        )

target_link_libraries(${TOP_MODULE} PRIVATE SystemC::systemc)
target_link_options(${TOP_MODULE} PRIVATE -pthread)
