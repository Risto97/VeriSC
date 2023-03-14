cmake_minimum_required(VERSION 3.8)
project(nmi VERSION 1.0)

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

target_include_directories(${PROJECT_NAME} 
    PRIVATE "${PROJECT_SOURCE_DIR}/inc"
    )

target_link_libraries(${PROJECT_NAME} 
                PRIVATE ${LIBS}
                )

install (FILES nmi.h
         DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
         COMPONENT dev)
install (DIRECTORY ${PROJECT_SOURCE_DIR}/inc/
         DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/nmi
         COMPONENT dev
         FILES_MATCHING PATTERN "*.h")
install (DIRECTORY ${PROJECT_SOURCE_DIR}/inc/
         DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/nmi
         COMPONENT dev
         FILES_MATCHING PATTERN "*.hpp")
