cmake_minimum_required(VERSION 3.8)
project(apb_lib VERSION 1.0)

set(CMAKE_CXX_FLAGS "-fPIC -O3 -DNDEBUG")

include(${PROJECT_SOURCE_DIR}/../cmake/get_libs.cmake)

# Create a new library
add_library(${PROJECT_NAME} OBJECT)

target_sources(${PROJECT_NAME} 
                # ENV SOURCES
                PRIVATE "src/apb_agent.cpp"
                PRIVATE "src/apb_driver.cpp"
                PRIVATE "src/apb_monitor.cpp"
                PRIVATE "src/apb_rw.cpp"
                )

target_include_directories(${PROJECT_NAME} 
                PUBLIC "${PROJECT_SOURCE_DIR}/inc"
                PUBLIC ${LIB_INCLUDES}
                )

install (FILES apb.h
         DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
         COMPONENT dev)
install (DIRECTORY ${PROJECT_SOURCE_DIR}/inc/
         DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/apb
         COMPONENT dev
         FILES_MATCHING PATTERN "*.h")
install (DIRECTORY ${PROJECT_SOURCE_DIR}/inc/
         DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/apb
         COMPONENT dev
         FILES_MATCHING PATTERN "*.hpp")

#
# target_link_libraries(${PROJECT_NAME} 
#                 PRIVATE ${LIBS}
#                 )
