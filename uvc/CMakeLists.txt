cmake_minimum_required(VERSION 3.20)
project(uvc)

add_subdirectory(apb)
add_subdirectory(nmi)
add_subdirectory(i2c)
add_subdirectory(common)

add_library(${PROJECT_NAME} SHARED)

set(CMAKE_VERBOSE_MAKEFILE ON)
include(${PROJECT_SOURCE_DIR}/cmake/get_libs.cmake)
target_link_libraries(${PROJECT_NAME}
                        apb_lib
                        nmi_lib
                        i2c_lib
                        common_lib
                        ${LIBS}
                        )

install(TARGETS ${PROJECT_NAME}
    DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  )

set(CMAKE_VERBOSE_MAKEFILE OFF)
