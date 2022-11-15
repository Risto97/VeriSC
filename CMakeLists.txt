cmake_minimum_required(VERSION 3.12)
project(SC_UVM_env VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    message(WARNING "Please define installation directonry by calling cmake with -DCMAKE_INSTALL_PREFIX=<installation-path>
    Using default directory ${PROJECT_BINARY_DIR}")
    set(CMAKE_INSTALL_PREFIX ${PROJECT_BINARY_DIR})
endif()

include(ExternalProject)

include("${PROJECT_SOURCE_DIR}/gcc/check_gcc_version.cmake")
message(STATUS "FROM SCRIPT: ${CXX_COMPILER}")
message(STATUS "FROM SCRIPT: ${C_COMPILER}")
ExternalProject_Add(gcc_install
            DOWNLOAD_COMMAND ""
            PREFIX ${PROJECT_BINARY_DIR}
            SOURCE_DIR ${PROJECT_SOURCE_DIR}/gcc
            CMAKE_ARGS
                -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
                -DCOMPILE_GCC=${COMPILE_GCC}
                -DDOWNLOAD_GCC_BINARY=${DDOWNLOAD_GCC_BINARY}
            INSTALL_COMMAND ""
            )

set(INSTALL_DIRECTORY ${CMAKE_INSTALL_PREFIX}/${GCC_VERSION})
ExternalProject_Add(tools_install
            DOWNLOAD_COMMAND ""
            PREFIX ${PROJECT_BINARY_DIR}
            SOURCE_DIR "${PROJECT_SOURCE_DIR}/tools"
            DEPENDS gcc_install
            BUILD_ALWAYS 1
            CMAKE_ARGS
                -DCMAKE_INSTALL_PREFIX=${INSTALL_DIRECTORY}
                -DCMAKE_CXX_COMPILER=${CXX_COMPILER}
                -DCMAKE_C_COMPILER=${C_COMPILER}
            )


ExternalProject_Add(uvc
            DOWNLOAD_COMMAND ""
            PREFIX ${PROJECT_BINARY_DIR}
            SOURCE_DIR "${PROJECT_SOURCE_DIR}/uvc"
            BUILD_ALWAYS 1
            CMAKE_ARGS
                -DCMAKE_INSTALL_PREFIX=${INSTALL_DIRECTORY}/uvc
                -DLIBS_CFG_DIR=${INSTALL_DIRECTORY}
                -DCMAKE_INSTALL_LIBDIR=lib-linux64
                -DCMAKE_INSTALL_INCLUDEDIR=include
                -DCMAKE_CXX_STANDARD=17
                -DCMAKE_CXX_COMPILER=${CXX_COMPILER}
                -DCMAKE_C_COMPILER=${C_COMPILER}
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

install(
    FILES "${PROJECT_SOURCE_DIR}/cmake/verilator.cmake"
    DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake"
  )
install(
    DIRECTORY "${PROJECT_SOURCE_DIR}/cmake/synopsys"
    DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake"
  )
  
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
