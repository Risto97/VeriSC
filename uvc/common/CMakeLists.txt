cmake_minimum_required(VERSION 3.8)
project(common VERSION 1.0)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE
   "${CMAKE_CURRENT_SOURCE_DIR}/inc"
   )

target_link_libraries(${PROJECT_NAME} 
    INTERFACE ${LIBS}
    )

install (DIRECTORY ${PROJECT_SOURCE_DIR}/inc/
         DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
         COMPONENT dev
         FILES_MATCHING PATTERN "*.h")
install (DIRECTORY ${PROJECT_SOURCE_DIR}/inc/
         DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
         COMPONENT dev
         FILES_MATCHING PATTERN "*.hpp")
