cmake_minimum_required(VERSION 3.12)

project(SC_UVM_env VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_VERBOSE_MAKEFILE ON)
# set(CMAKE_VERBOSE_MAKEFILE ON)
# set(CMAKE_FIND_DEBUG_MODE 1)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


include(ExternalProject)

ExternalProject_Add(gcc_install
            DOWNLOAD_COMMAND ""
            PREFIX ${PROJECT_BINARY_DIR}
            SOURCE_DIR "${PROJECT_SOURCE_DIR}/gcc"
            CMAKE_ARGS
                -DCMAKE_INSTALL_PREFIX=${PROJECT_BINARY_DIR}/installs
            INSTALL_COMMAND ""
            )

ExternalProject_Add(tools_install
            DOWNLOAD_COMMAND ""
            PREFIX ${PROJECT_BINARY_DIR}
            SOURCE_DIR "${PROJECT_SOURCE_DIR}/tools"
            DEPENDS gcc_install
            CMAKE_ARGS
                -DCMAKE_INSTALL_PREFIX=${PROJECT_BINARY_DIR}/installs
                -DCMAKE_CXX_COMPILER=${PROJECT_BINARY_DIR}/installs/gcc-12.2.0/bin/g++ #TODO not correct
                -DCMAKE_C_COMPILER=${PROJECT_BINARY_DIR}/installs/gcc-12.2.0/bin/gcc
            )


# set(GROUP_INSTALL_DIR ${PROJECT_BINARY_DIR}/installs/${CMAKE_CXX_COMPILER_VERSION})
# set(UVC_INSTALL_DIR ${GROUP_INSTALL_DIR}/uvc/)
ExternalProject_Add(uvc
            # GIT_REPOSITORY https://github.com/Risto97/systemc_uvm_uvc.git
            # GIT_TAG master
            DOWNLOAD_COMMAND ""
            PREFIX ${PROJECT_BINARY_DIR}
            SOURCE_DIR "${PROJECT_SOURCE_DIR}/uvc"
            CMAKE_ARGS
                -DCMAKE_INSTALL_PREFIX=${PROJECT_BINARY_DIR}/installs
                -DCMAKE_INSTALL_LIBDIR=lib-linux64
                -DCMAKE_INSTALL_INCLUDEDIR=include
                -DCMAKE_CXX_STANDARD=17
                -DCMAKE_CXX_COMPILER=${PROJECT_BINARY_DIR}/installs/gcc-12.2.0/bin/g++
                -DCMAKE_C_COMPILER=${PROJECT_BINARY_DIR}/installs/gcc-12.2.0/bin/gcc
            DEPENDS gcc_install tools_install
            )
include(GNUInstallDirs)

configure_file(EnvConfig.h.in EnvConfig.h)


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
