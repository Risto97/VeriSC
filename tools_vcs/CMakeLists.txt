cmake_minimum_required(VERSION 3.21)
project(tools_install_vcs)

include(ExternalProject)

if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
		cmake_policy(SET CMP0135 NEW)
endif()

set(GROUP_INSTALL_DIR ${CMAKE_INSTALL_PREFIX})

if(NOT DEFINED VCS_HOME OR VCS_HOME STREQUAL "")
    if(DEFINED ENV{VCS_HOME})
        message(STATUS "Using environment variable VCS_HOME: ${VCS_HOME}")
        set(VCS_HOME $ENV{VCS_HOME})
    else()
        message(FATAL_ERROR "Set environment variable VCS_HOME or run cmake with -DVCS_HOME parameter, cmake parameter has priority")

    endif()
endif()

set(SYSTEMC_VERSION 2.3.3)
set(SCV_VERSION 2.0.1)
set(UVM_VERSION 1.0-beta4)
set(VERILATOR_VERSION 4.228)
set(FC4SC_VERSION master)
set(ICSC_COMPILER_VERSION master)

set(ICSC_COMPILER_INSTALL_DIR ${GROUP_INSTALL_DIR}/systemc-compiler-${ICSC_COMPILER_VERSION})

set(SYSTEMC_INSTALL_DIR ${GROUP_INSTALL_DIR}/systemc-${SYSTEMC_VERSION})
set(SYSTEMC_SRC_DIR ${VCS_HOME}/etc/systemc/accellera_install/systemc233-gcc9/)
ExternalProject_Add(systemc
    DOWNLOAD_COMMAND ""
    SOURCE_DIR ${SYSTEMC_SRC_DIR}
    PREFIX ${PROJECT_BINARY_DIR}
    INSTALL_DIR ${SYSTEMC_INSTALL_DIR}
    UPDATE_COMMAND ""
    PATCH_COMMAND ""
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ${CMAKE_COMMAND} -E copy_directory ${SYSTEMC_SRC_DIR} ${SYSTEMC_INSTALL_DIR}
    )

set(SCV_INSTALL_DIR ${GROUP_INSTALL_DIR}/scv-${SCV_VERSION})
set(SCV_SRC_INC_DIR ${VCS_HOME}/include/scv-2.0/)
ExternalProject_Add(scv
    SOURCE_DIR ${SCV_SRC_INC_DIR}
    PREFIX ${PROJECT_BINARY_DIR}
    UPDATE_COMMAND ""
    PATCH_COMMAND ""
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND 
    ${CMAKE_COMMAND} -E copy_directory ${SCV_SRC_INC_DIR} ${SCV_INSTALL_DIR}/include &&
    ${CMAKE_COMMAND} -E make_directory ${SCV_INSTALL_DIR}/lib-linux64 &&
    ${CMAKE_COMMAND} -E copy ${VCS_HOME}/linux64/lib/libscv-2.0-sysc233-gcc9-64.a ${SCV_INSTALL_DIR}/lib-linux64/libscv.a
    )

set(UVM_INSTALL_DIR ${GROUP_INSTALL_DIR}/uvm-systemc-${UVM_VERSION})
set(UVM_SRC_DIR ${PROJECT_BINARY_DIR}/src/uvm-systemc)
set(VG_GNU_PACKAGE "${VCS_HOME}/gnu/linux64/")
set(VCS_PATH_ENV "${VCS_HOME}/gnu/linux64/binutils-2.33.1_64/bin/:${VCS_HOME}/gnu/linux64/gcc-9.2.0_64/bin/:$ENV{PATH}")
set(VCS_LIB_PATH "${VCS_HOME}/gnu/linux64/gcc-9.2.0_64/lib64/:${VCS_HOME}/gnu/linux64/gcc-9.2.0_64/lib/:$ENV{LD_LIBRARY_PATH}")
ExternalProject_Add(uvm-systemc
    URL https://www.accellera.org/images/downloads/standards/systemc/uvm-systemc-${UVM_VERSION}.tar.gz
    PREFIX ${PROJECT_BINARY_DIR}
    CONFIGURE_COMMAND  
    ${CMAKE_COMMAND} -E env "VG_GNU_PACKAGE=${VG_GNU_PACKAGE}"
    ${CMAKE_COMMAND} -E env "PATH=${VCS_PATH_ENV}"
    ${CMAKE_COMMAND} -E env "LD_LIBRARY_PATH=${VCS_LIB_PATH}"
    ${UVM_SRC_DIR}/configure CC=${CMAKE_C_COMPILER} CXX=${CMAKE_CXX_COMPILER} --with-systemc=${SYSTEMC_INSTALL_DIR} CXXFLAGS=-std=c++${CMAKE_CXX_STANDARD} --prefix=${UVM_INSTALL_DIR}

    BUILD_COMMAND  
    ${CMAKE_COMMAND} -E env "VG_GNU_PACKAGE=${VG_GNU_PACKAGE}"
    ${CMAKE_COMMAND} -E env "PATH=${VCS_PATH_ENV}"
    ${CMAKE_COMMAND} -E env "LD_LIBRARY_PATH=${VCS_LIB_PATH}"
        $(MAKE) 
    DEPENDS systemc
    )

set(FC4SC_INSTALL_DIR ${GROUP_INSTALL_DIR}/fc4sc-${FC4SC_VERSION})
set(FC4SC_SRC_DIR ${PROJECT_BINARY_DIR}/src/fc4sc)
ExternalProject_Add(fc4sc
    GIT_REPOSITORY https://github.com/amiq-consulting/fc4sc.git
    GIT_TAG ${FC4SC_VERSION}
    PREFIX ${PROJECT_BINARY_DIR}
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ${CMAKE_COMMAND} -E copy_directory ${FC4SC_SRC_DIR} ${FC4SC_INSTALL_DIR}
    )
#
set(TAG "VCS")
# configure_file(${PROJECT_SOURCE_DIR}/source.sh.in ${PROJECT_BINARY_DIR}/source.sh)
configure_file(${PROJECT_SOURCE_DIR}/libs_conf.cmake.in ${PROJECT_BINARY_DIR}/libs_conf_${TAG}.cmake)
#
# install(FILES ${PROJECT_BINARY_DIR}/source.sh
#         DESTINATION ${GROUP_INSTALL_DIR})
install(FILES ${PROJECT_BINARY_DIR}/libs_conf_${TAG}.cmake
        DESTINATION ${GROUP_INSTALL_DIR}
        RENAME libs_conf.cmake
        )

install(FILES ${PROJECT_SOURCE_DIR}/vcs_home_resolve.cmake
        DESTINATION ${GROUP_INSTALL_DIR}
        )
