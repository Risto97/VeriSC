cmake_minimum_required(VERSION 3.12)
project(SC_UVM_env VERSION 1.0.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    message(WARNING "Please define installation directonry by calling cmake with -DCMAKE_INSTALL_PREFIX=<installation-path>
    Using default directory ${PROJECT_BINARY_DIR}")
    set(CMAKE_INSTALL_PREFIX ${PROJECT_BINARY_DIR})
endif()

set(CMAKE_CXX_STANDARD 17)

include(ExternalProject)

include("${PROJECT_SOURCE_DIR}/gcc/check_gcc_version.cmake")
message(STATUS "FROM SCRIPT: ${CXX_COMPILER}")
message(STATUS "FROM SCRIPT: ${C_COMPILER}")
ExternalProject_Add(gcc_install
            DOWNLOAD_COMMAND ""
            PREFIX ${PROJECT_BINARY_DIR}
            SOURCE_DIR ${PROJECT_SOURCE_DIR}/gcc
            BUILD_ALWAYS 1
            CMAKE_ARGS
                -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
                -DGCC_VERSION=${GCC_VERSION}
                -DCOMPILE_GCC=${COMPILE_GCC}
                -DDOWNLOAD_GCC_BINARY=${DOWNLOAD_GCC_BINARY}
                -DCMAKE_VERBOSE_MAKEFILE=${CMAKE_VERBOSE_MAKEFILE}
            )

set(INSTALL_DIRECTORY ${CMAKE_INSTALL_PREFIX}/open)
ExternalProject_Add(tools_install
            DOWNLOAD_COMMAND ""
            PREFIX ${PROJECT_BINARY_DIR}
            SOURCE_DIR "${PROJECT_SOURCE_DIR}/tools"
            BUILD_ALWAYS 1
            CMAKE_ARGS
                -DCMAKE_INSTALL_PREFIX=${INSTALL_DIRECTORY}
                -DCMAKE_CXX_COMPILER=${CXX_COMPILER}
                -DCMAKE_C_COMPILER=${C_COMPILER}
                -DCMAKE_VERBOSE_MAKEFILE=${CMAKE_VERBOSE_MAKEFILE}
                -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}
            DEPENDS gcc_install
            )


ExternalProject_Add(uvc
            DOWNLOAD_COMMAND ""
            PREFIX ${PROJECT_BINARY_DIR}
            SOURCE_DIR "${PROJECT_SOURCE_DIR}/uvc"
            BUILD_ALWAYS 1
            CMAKE_ARGS
                -DCMAKE_INSTALL_PREFIX=${INSTALL_DIRECTORY}/uvc
                -DLIBS_CFG_DIR=${INSTALL_DIRECTORY}
                -DINSTALL_GROUP=OPEN
                -DCMAKE_INSTALL_LIBDIR=lib-linux64
                -DCMAKE_INSTALL_INCLUDEDIR=include
                -DCMAKE_CXX_STANDARD=14
                -DCMAKE_CXX_COMPILER=${CXX_COMPILER}
                -DCMAKE_C_COMPILER=${C_COMPILER}
                -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}
                -DCMAKE_VERBOSE_MAKEFILE=${CMAKE_VERBOSE_MAKEFILE}
            DEPENDS gcc_install tools_install
            )

if(NOT DEFINED VCS_HOME AND NOT DEFINED ENV{VCS_HOME})
    message(WARNING "VCS libraries will not be compiled, VCS will not be used, run cmake with -DVCS_HOME=<path to vcs> or set environment variable VCS_HOME")
    set(USE_VCS FALSE)
else()
    set(USE_VCS TRUE)
    set(VCS_C_COMPILER /eda/synopsys/2021-22/RHELx86/VCS_2021.09-SP1/gnu/linux64/gcc-9.2.0_64/bin/gcc)
    set(VCS_CXX_COMPILER /eda/synopsys/2021-22/RHELx86/VCS_2021.09-SP1/gnu/linux64/gcc-9.2.0_64/bin/g++)
    set(VCS_INSTALL_DIRECTORY ${CMAKE_INSTALL_PREFIX}/vcs)
    ExternalProject_Add(tools_install_vcs
                DOWNLOAD_COMMAND ""
                PREFIX ${PROJECT_BINARY_DIR}
                SOURCE_DIR "${PROJECT_SOURCE_DIR}/tools_vcs"
                BUILD_ALWAYS 1
                CMAKE_ARGS
                    -DCMAKE_INSTALL_PREFIX=${VCS_INSTALL_DIRECTORY}
                    -DCMAKE_CXX_COMPILER=${VCS_CXX_COMPILER}
                    -DCMAKE_C_COMPILER=${VCS_C_COMPILER}
                    -DVCS_HOME=${VCS_HOME}
                    -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}
                    -DCMAKE_VERBOSE_MAKEFILE=${CMAKE_VERBOSE_MAKEFILE}
                )

    set(VG_GNU_PACKAGE "${VCS_HOME}/gnu/linux64/")
    set(VCS_PATH_ENV "${VCS_HOME}/gnu/linux64/binutils-2.33.1_64/bin/:${VCS_HOME}/gnu/linux64/gcc-9.2.0_64/bin/:$ENV{PATH}")
    set(VCS_LIB_PATH "${VCS_HOME}/gnu/linux64/gcc-9.2.0_64/lib64/:${VCS_HOME}/gnu/linux64/gcc-9.2.0_64/lib/:$ENV{LD_LIBRARY_PATH}")
    set(CMAKE_VERBOSE_MAKEFILE TRUE)
    set(VCS_LINKER ${VCS_HOME}/gnu/linux64/binutils-2.33.1_64/bin/ld)
    ExternalProject_Add(uvc_vcs
                DOWNLOAD_COMMAND ""
                PREFIX ${PROJECT_BINARY_DIR}
                SOURCE_DIR "${PROJECT_SOURCE_DIR}/uvc"
                BUILD_ALWAYS 1
                CMAKE_ARGS
                    -DCMAKE_INSTALL_PREFIX=${VCS_INSTALL_DIRECTORY}/uvc
                    -DLIBS_CFG_DIR=${VCS_INSTALL_DIRECTORY}
                    -DINSTALL_GROUP=VCS
                    -DCMAKE_INSTALL_LIBDIR=lib-linux64
                    -DCMAKE_INSTALL_INCLUDEDIR=include
                    -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}
                    -DCMAKE_LINKER=${VCS_LINKER}
                    -DCMAKE_CXX_COMPILER=${VCS_CXX_COMPILER}
                    -DCMAKE_C_COMPILER=${VCS_C_COMPILER}
                    -DCMAKE_VERBOSE_MAKEFILE=${CMAKE_VERBOSE_MAKEFILE}
                DEPENDS tools_install_vcs
                )
    install(
        DIRECTORY "${PROJECT_SOURCE_DIR}/cmake/synopsys"
        DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake"
      )

endif()
                #
include(GNUInstallDirs)

# configure_file(${PROJECT_SOURCE_DIR}/cmake/sc_uvm_env_setup.cmake.in ${PROJECT_BINARY_DIR}/sc_uvm_env_setup.cmake)
configure_file(
    ${PROJECT_SOURCE_DIR}/cmake/sc_uvm_env_setup.cmake.in
    ${CMAKE_INSTALL_PREFIX}/sc_uvm_env_setup.cmake
    @ONLY
    )
# install(FILES ${PROJECT_SOURCE_DIR}/cmake/sc_uvm_env_setup.cmake
#     DESTINATION ${CMAKE_INSTALL_PREFIX}
#     )


set(config_install_dir "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")
message(STATUS "INSTALL_LIBDIR: ${config_install_dir}")

include(CMakePackageConfigHelpers)

install(
    FILES "${PROJECT_SOURCE_DIR}/cmake/verilator.cmake"
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
