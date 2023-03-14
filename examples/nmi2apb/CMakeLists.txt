find_package(veriSC CONFIG REQUIRED
    PATHS $ENV{VERISC_HOME}
    NO_DEFAULT_PATH
    )

cmake_minimum_required(VERSION ${CMAKE_MIN_VERSION})
project(nmi2apb_env VERSION 1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_subdirectory(rtl "rtllib")

add_executable(${PROJECT_NAME}
    "${PROJECT_SOURCE_DIR}/sc_main.cpp"
    "${PROJECT_SOURCE_DIR}/env.cpp"
    "${PROJECT_SOURCE_DIR}/scoreboard.cpp"
    "${PROJECT_SOURCE_DIR}/nmi_sequence.cpp"
    )

target_include_directories(${PROJECT_NAME} PRIVATE
    ${PROJECT_SOURCE_DIR}
    )

target_compile_options(${PROJECT_NAME} PRIVATE
    -O3
    -DNDEBUG
    )

make_rtl(rtl_lib nmi2apb_rtl)
make_tb(${PROJECT_NAME} 
    RTL_LIBS rtl_lib
    )
