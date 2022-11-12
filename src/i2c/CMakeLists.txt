cmake_minimum_required(VERSION 3.8)
project(I2C_lib VERSION 1.0)

set(CMAKE_VERBOSE_MAKEFILE ON)
# set(CMAKE_FIND_DEBUG_MODE 1)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS "-DNDEBUG -fPIC")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# SystemC dependencies
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# Find SystemC using SystemC's CMake integration
# set(CMAKE_PREFIX_PATH "${SYSTEMC}")
# find_package(SystemCLanguage CONFIG REQUIRED)
# if (NOT SystemCLanguage_FOUND)
#     message(FATAL_ERROR "SystemC not found")
# endif()
#
set(LIB_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/inc")

# Create a new library
add_library(${PROJECT_NAME} OBJECT
            inc/i2c.h
            inc/i2c_if.h
            inc/i2c_agent.h
            inc/i2c_driver.h
            inc/i2c_monitor.h
            inc/i2c_modes.h
            # inc/i2c_sequence.h
            inc/i2c_sequencer.h
            inc/i2c_rw.h
            )# SCT Source file that needs to be compiled

# SCT Source file that needs to be compiled
# set(SCT_DIR "$ENV{SCT_HOME}/components/common/sctcommon/")
# set(SCT_SOURCES "${SCT_DIR}/sct_property.cpp")
target_sources(${PROJECT_NAME} 
                # PRIVATE ${SCT_SOURCES}

                # ENV SOURCES
                PRIVATE "src/i2c_agent.cpp"
                PRIVATE "src/i2c_driver.cpp"
                PRIVATE "src/i2c_monitor.cpp"
                # PRIVATE "src/i2c_sequence.cpp"
                PRIVATE "src/i2c_rw.cpp"
                PRIVATE "src/i2c_if.cpp"
                )

# Add SCV directory with libscv.so to path
target_link_directories(${PROJECT_NAME}
                # PRIVATE "$ENV{SCV}/objdir/src/scv/.libs"
                PRIVATE "$ENV{SYSTEMC_UVM}/lib-linux64"
                PRIVATE "$ENV{SYSTEMC_HOME}/lib-linux64"
                )

target_include_directories(${PROJECT_NAME} 
                # PRIVATE "$ENV{SCV}/src"
                PRIVATE "$ENV{SYSTEMC_UVM}/include"
                PRIVATE "$ENV{FC4SC}/includes"
                PRIVATE "$ENV{SYSTEMC_HOME}/include"
                # PRIVATE "${SCT_DIR}"

                # ENV INCLUDES
                PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/inc"
                )

target_link_libraries(${PROJECT_NAME} 
                PRIVATE systemc
                # PRIVATE scv
                PRIVATE uvm-systemc 
                )

install(TARGETS ${PROJECT_NAME}
    DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  )

# EXPORT VARIABLES TO PARENT
set(LIB_INCLUDE_DIR ${LIB_INCLUDE_DIR} PARENT_SCOPE)
