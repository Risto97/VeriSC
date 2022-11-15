cmake_minimum_required(VERSION 3.8)
project(nmi2apb_env VERSION 1.0)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_subdirectory(rtl "rtllib")

add_library(${PROJECT_NAME} INTERFACE)

target_sources(${PROJECT_NAME} INTERFACE
    "${PROJECT_SOURCE_DIR}/sc_main.cpp"
    "${PROJECT_SOURCE_DIR}/env.cpp"
    "${PROJECT_SOURCE_DIR}/scoreboard.cpp"

    "${PROJECT_SOURCE_DIR}/nmi_sequence.cpp"
    )

target_include_directories(${PROJECT_NAME} INTERFACE
    ${PROJECT_SOURCE_DIR}
    )

target_compile_options(${PROJECT_NAME} INTERFACE
    -O3
    -DNDEBUG
    )

set(CMAKE_FIND_DEBUG_MODE True)
find_file(libs_conf_cmake "libs_conf.cmake"
    PATHS "$ENV{SC_UVM_ENV_HOME}/${CMAKE_CXX_COMPILER_VERSION}" NO_DEFAULT_PATH)
include(${libs_conf_cmake})
find_file(verilator_cmake "verilator.cmake"
    PATHS "$ENV{SC_UVM_ENV_HOME}/*" NO_DEFAULT_PATH)
set(CMAKE_FIND_DEBUG_MODE False)
include(${verilator_cmake})
