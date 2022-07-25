cmake_minimum_required(VERSION 3.8)
project(APB_lib VERSION 1.0)

 message(STATUS "APB CXX STANDARD ${CMAKE_CXX_STANDARD}")

# set(CMAKE_VERBOSE_MAKEFILE ON)
# set(CMAKE_FIND_DEBUG_MODE 1)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS "")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# SystemC dependencies
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# Find SystemC using SystemC's CMake integration
set(CMAKE_PREFIX_PATH "${SYSTEMC}")
find_package(SystemCLanguage CONFIG REQUIRED)
if (NOT SystemCLanguage_FOUND)
    message(FATAL_ERROR "SystemC not found")
endif()


# Create a new library
add_library(${PROJECT_NAME}
            inc/apb.h
            inc/apb_if.h
            inc/apb_modes.h
            inc/apb_agent.h
            inc/apb_driver.h
            inc/apb_monitor.h
            inc/apb_sequence.h
            inc/apb_sequencer.h
            inc/apb_rw.h
            inc/univ_subscriber.h
            )# SCT Source file that needs to be compiled

# SCT Source file that needs to be compiled
set(SCT_DIR "$ENV{SCT_HOME}/components/common/sctcommon/")
set(SCT_SOURCES "${SCT_DIR}/sct_property.cpp")
target_sources(${PROJECT_NAME} 
                PRIVATE ${SCT_SOURCES}

                # ENV SOURCES
                PRIVATE "src/apb_agent.cpp"
                PRIVATE "src/apb_driver.cpp"
                PRIVATE "src/apb_monitor.cpp"
                PRIVATE "src/apb_sequence.cpp"
                PRIVATE "src/apb_rw.cpp"
                )

# Add SCV directory with libscv.so to path
target_link_directories(${PROJECT_NAME}
                PRIVATE "$ENV{SCV}/objdir/src/scv/.libs"
                PRIVATE "$ENV{SYSTEMC_UVM}/lib-linux64"
                PRIVATE "$ENV{SYSTEMC_UVM}/lib-linux64"
                )

target_include_directories(${PROJECT_NAME} 
                PRIVATE "$ENV{SCV}/src"
                PRIVATE "$ENV{SYSTEMC_UVM}/include"
                PRIVATE "$ENV{FC4SC}/includes"
                PRIVATE "${SCT_DIR}"

                # ENV INCLUDES
                PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/inc"
                )

target_link_libraries(${PROJECT_NAME} 
                PRIVATE rtl
                PRIVATE SystemC::systemc
                PRIVATE scv
                PRIVATE uvm-systemc 
                )

install(TARGETS ${PROJECT_NAME}
    DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  )
