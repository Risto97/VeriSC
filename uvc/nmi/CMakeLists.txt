cmake_minimum_required(VERSION 3.8)
project(nmi_lib VERSION 1.0)

set(CMAKE_CXX_FLAGS "-fPIC -O3 -DNDEBUG")

include(${PROJECT_SOURCE_DIR}/../cmake/get_libs.cmake)

# Create a new library
add_library(${PROJECT_NAME} OBJECT)

target_sources(${PROJECT_NAME} 
                # PRIVATE ${SCT_SOURCES}

                # ENV SOURCES
                PRIVATE "src/nmi_agent.cpp"
                PRIVATE "src/nmi_driver.cpp"
                PRIVATE "src/nmi_monitor.cpp"
                PRIVATE "src/nmi_rw.cpp"
                )

# Add SCV directory with libscv.so to path
target_include_directories(${PROJECT_NAME} 
                PUBLIC "${PROJECT_SOURCE_DIR}/inc"
                PUBLIC ${LIB_INCLUDES}
                )

install (FILES 
        ${PROJECT_SOURCE_DIR}/inc/nmi.h
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        COMPONENT dev)
#
# target_link_libraries(${PROJECT_NAME} 
#                 PRIVATE ${LIBS}
#                 )

