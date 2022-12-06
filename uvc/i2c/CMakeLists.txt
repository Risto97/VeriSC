cmake_minimum_required(VERSION 3.8)
project(i2c_lib VERSION 1.0)

set(CMAKE_CXX_FLAGS "-fPIC -O3 -DNDEBUG")

include(${PROJECT_SOURCE_DIR}/../cmake/get_libs.cmake)

# Create a new library
add_library(${PROJECT_NAME} OBJECT)

target_sources(${PROJECT_NAME} 

                # ENV SOURCES
                PRIVATE "src/i2c_agent.cpp"
                PRIVATE "src/i2c_driver.cpp"
                PRIVATE "src/i2c_monitor.cpp"
                # PRIVATE "src/i2c_sequence.cpp"
                PRIVATE "src/i2c_rw.cpp"
                PRIVATE "src/i2c_if.cpp"
                )

target_include_directories(${PROJECT_NAME} 
                PUBLIC "${PROJECT_SOURCE_DIR}/inc"
                PUBLIC "${PROJECT_SOURCE_DIR}/../common/inc"
                PUBLIC ${LIB_INCLUDES}
                )

install (FILES i2c.h
         DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
         COMPONENT dev)
install (DIRECTORY ${PROJECT_SOURCE_DIR}/inc/
         DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/i2c
         COMPONENT dev
         FILES_MATCHING PATTERN "*.h")
install (DIRECTORY ${PROJECT_SOURCE_DIR}/inc/
         DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/i2c
         COMPONENT dev
         FILES_MATCHING PATTERN "*.hpp")
