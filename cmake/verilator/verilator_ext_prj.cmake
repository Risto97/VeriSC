cmake_minimum_required(VERSION 3.25)
project(${TARGET})

find_package(verilator HINTS ${VERILATOR_ROOT})
if (NOT verilator_FOUND)
  message(FATAL_ERROR "Verilator was not found. Either install it, or set the VERILATOR_ROOT environment variable")
endif()

foreach(ARG ${ARGUMENTS_LIST})
    string(REPLACE "VERILATE_" "" ARG_MOD ${ARG})
    list(APPEND FORWARDED_ARGS ${ARG_MOD} ${VERILATE_${ARG}})
endforeach()

add_library(${TARGET})
verilate(${TARGET}
    ${FORWARDED_ARGS}
    )

if(VERILATE_SYSTEMC)
    set(__VERISC_REL_ROOT "${CMAKE_CURRENT_LIST_DIR}/../../")
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)

    find_package(SystemCLanguage CONFIG REQUIRED
        PATHS ${__VERISC_REL_ROOT}/open/*
        NO_DEFAULT_PATH
        )

    target_link_libraries(${TARGET} PUBLIC SystemC::systemc)
    target_link_options(${TARGET} PUBLIC SystemC::systemc -pthread)
endif()


