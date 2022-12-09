find_file(setup_fn "sc_uvm_env_setup.cmake"
    PATHS "$ENV{SC_UVM_ENV_HOME}" NO_DEFAULT_PATH)
include(${setup_fn})

cmake_minimum_required(VERSION ${CMAKE_MIN_VERSION})
project(crave_rand VERSION 1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_VERBOSE_MAKEFILE ON)

add_library(${PROJECT_NAME} INTERFACE)

target_sources(${PROJECT_NAME} INTERFACE
    "${PROJECT_SOURCE_DIR}/sc_main.cpp"
    )

target_include_directories(${PROJECT_NAME} INTERFACE
    ${PROJECT_SOURCE_DIR}
    )

target_compile_options(${PROJECT_NAME} INTERFACE
    -O3
    -DNDEBUG
    )

make_tb(${PROJECT_NAME} "")
