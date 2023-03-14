find_package(veriSC CONFIG REQUIRED
    PATHS $ENV{VERISC_HOME}
    NO_DEFAULT_PATH
    )

cmake_minimum_required(VERSION ${CMAKE_MIN_VERSION})
project(simple_sc_verilog VERSION 1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_VERBOSE_MAKEFILE ON)

add_executable(${PROJECT_NAME}
    "${PROJECT_SOURCE_DIR}/sc_main.cpp"
    )

target_include_directories(${PROJECT_NAME} PRIVATE
    ${PROJECT_SOURCE_DIR}
    )

target_compile_options(${PROJECT_NAME} PRIVATE
    -O3
    -DNDEBUG
    )

if(VERBOSE)
    target_compile_options(${PROJECT_NAME} PRIVATE -DVERBOSE)
endif()


add_subdirectory(rtl "adder_rtl")

make_rtl(adder_lib adder_rtl)

make_tb(${PROJECT_NAME} 
    RTL_LIBS adder_lib
    DEPS SystemC::systemc
    )
