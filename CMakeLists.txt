cmake_minimum_required(VERSION 3.12)

project(SC_UVM_env VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_VERBOSE_MAKEFILE ON)
# set(CMAKE_VERBOSE_MAKEFILE ON)
# set(CMAKE_FIND_DEBUG_MODE 1)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS "-DNDEBUG -fPIC")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# include("cmake/find_systemc.cmake")

add_subdirectory(tools)
# add_subdirectory(uvc)

include(ExternalProject)

set(GROUP_INSTALL_DIR ${PROJECT_BINARY_DIR}/installs/${CMAKE_CXX_COMPILER_VERSION})
set(UVC_INSTALL_DIR ${GROUP_INSTALL_DIR}/uvc/)
ExternalProject_Add(uvc
            # GIT_REPOSITORY https://github.com/Risto97/systemc_uvm_uvc.git
            # GIT_TAG master
            DOWNLOAD_COMMAND ""
            PREFIX ${PROJECT_BINARY_DIR}
            SOURCE_DIR "${PROJECT_SOURCE_DIR}/uvc"
            CMAKE_ARGS
                -DCMAKE_INSTALL_PREFIX=${UVC_INSTALL_DIR}
                -DCMAKE_INSTALL_LIBDIR=lib-linux64
                -DCMAKE_INSTALL_INCLUDEDIR=include
                -DCMAKE_CXX_STANDARD=17
            DEPENDS systemc scv fc4sc uvm-systemc
            )
include(GNUInstallDirs)

configure_file(EnvConfig.h.in EnvConfig.h)

# add_subdirectory(uvc)

# set(SCT_DIR "$ENV{SCT_HOME}/components/common/sctcommon/")
# list(APPEND EXTRA_SOURCES "${SCT_DIR}/sct_property.cpp")

# add_library(${PROJECT_NAME} SHARED
#     $<TARGET_OBJECTS:APB_lib>
#     # $<TARGET_OBJECTS:NMI_lib>
#     # $<TARGET_OBJECTS:I2C_lib>
#     ${EXTRA_SOURCES}
#     )

# target_link_libraries(${PROJECT_NAME}
            # PRIVATE SystemC::systemc
            # )

set(config_install_dir "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")
message(STATUS "INSTALL_LIBDIR: ${config_install_dir}")

include(CMakePackageConfigHelpers)
configure_file( 
      "cmake/SC_UVM_envConfig.cmake.in" 
      # "${CMAKE_CURRENT_BINARY_DIR}/SC_UVM_envConfig.cmake"
      "SC_UVM_envConfig.cmake"
      # INSTALL_DESTINATION "${PROJECT_BINARY_DIR}"
  )

install(
  FILES "${CMAKE_CURRENT_BINARY_DIR}/SC_UVM_envConfig.cmake"
  DESTINATION "${PROJECT_BINARY_DIR}"
  )
  
# install(TARGETS ${PROJECT_NAME}
#     DESTINATION "${CMAKE_INSTALL_LIBDIR}"
#   )

# uninstall target
# if(NOT TARGET uninstall)
#   configure_file(
#     "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
#     "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
#     IMMEDIATE @ONLY)
#
#   add_custom_target(uninstall
#     COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
# endif()
