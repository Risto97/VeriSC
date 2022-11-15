cmake_minimum_required(VERSION 3.20)
project(tools_install)

include(ExternalProject)

if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
		cmake_policy(SET CMP0135 NEW)
endif()

set(GROUP_INSTALL_DIR ${CMAKE_INSTALL_PREFIX})


set(SYSTEMC_VERSION 2.3.3)
set(SCV_VERSION 2.0.1)
set(UVM_VERSION 1.0-beta4)
set(VERILATOR_VERSION 4.228)
set(FC4SC_VERSION master)
set(ICSC_COMPILER_VERSION master)

set(ICSC_COMPILER_INSTALL_DIR ${GROUP_INSTALL_DIR}/systemc-compiler-${ICSC_COMPILER_VERSION})

set(SYSTEMC_INSTALL_DIR ${GROUP_INSTALL_DIR}/systemc-${SYSTEMC_VERSION})
ExternalProject_Add(systemc
    URL https://www.accellera.org/images/downloads/standards/systemc/systemc-${SYSTEMC_VERSION}.tar.gz
    PREFIX ${PROJECT_BINARY_DIR}
    INSTALL_DIR ${SYSTEMC_INSTALL_DIR}
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${SYSTEMC_INSTALL_DIR}
        -DCMAKE_INSTALL_LIBDIR=lib-linux64
        -DCMAKE_CXX_STANDARD=17
        -DENABLE_PHASE_CALLBACKS_TRACING=OFF
        -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
        -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
    )

set(SCV_INSTALL_DIR ${GROUP_INSTALL_DIR}/scv-${SCV_VERSION})
set(SCV_SRC_DIR ${PROJECT_BINARY_DIR}/src/scv)
ExternalProject_Add(scv
    URL https://www.accellera.org/images/downloads/standards/systemc/scv-${SCV_VERSION}.tar.gz
    PREFIX ${PROJECT_BINARY_DIR}
    CONFIGURE_COMMAND autoreconf -fiv ${SCV_SRC_DIR} && ${SCV_SRC_DIR}/configure CC=${CMAKE_C_COMPILER} CXX=${CMAKE_CXX_COMPILER} --with-systemc=${SYSTEMC_INSTALL_DIR} --prefix=${SCV_INSTALL_DIR}
    DEPENDS systemc
    )

set(UVM_INSTALL_DIR ${GROUP_INSTALL_DIR}/uvm-systemc-${UVM_VERSION})
set(UVM_SRC_DIR ${PROJECT_BINARY_DIR}/src/uvm-systemc)
ExternalProject_Add(uvm-systemc
    URL https://www.accellera.org/images/downloads/standards/systemc/uvm-systemc-${UVM_VERSION}.tar.gz
    PREFIX ${PROJECT_BINARY_DIR}
    CONFIGURE_COMMAND autoreconf -fiv ${UVM_SRC_DIR} && ${UVM_SRC_DIR}/configure CC=${CMAKE_C_COMPILER} CXX=${CMAKE_CXX_COMPILER} --with-systemc=${SYSTEMC_INSTALL_DIR} --prefix=${UVM_INSTALL_DIR}
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


set(VERILATOR_INSTALL_DIR ${GROUP_INSTALL_DIR}/verilator-${VERILATOR_VERSION})
set(VERILATOR_SRC_DIR ${PROJECT_BINARY_DIR}/src/verilator)
message(STATUS "VERILATOR_SRC_DIR ${VERILATOR_SRC_DIR}")
ExternalProject_Add(verilator
    GIT_REPOSITORY https://github.com/verilator/verilator.git
    GIT_TAG v${VERILATOR_VERSION}
    PREFIX ${PROJECT_BINARY_DIR}
    PATCH_COMMAND patch -N ${VERILATOR_SRC_DIR}/verilator-config.cmake.in -i ${PROJECT_SOURCE_DIR}/verilator-config.patch || true
    CONFIGURE_COMMAND  autoconf --output ${VERILATOR_SRC_DIR}/configure ${VERILATOR_SRC_DIR}/configure.ac && ${VERILATOR_SRC_DIR}/configure CC=${CMAKE_C_COMPILER} CXX=${CMAKE_CXX_COMPILER} --prefix=${VERILATOR_INSTALL_DIR}
    INSTALL_COMMAND make install && ${CMAKE_COMMAND} -E copy_directory ${VERILATOR_INSTALL_DIR}/share/verilator/include ${VERILATOR_INSTALL_DIR}/include
    )

set(TAG "OSCI")
configure_file(${PROJECT_SOURCE_DIR}/source.sh.in ${PROJECT_BINARY_DIR}/source.sh)
configure_file(${PROJECT_SOURCE_DIR}/libs_conf.cmake.in ${PROJECT_BINARY_DIR}/libs_conf_${TAG}.cmake)

install(FILES ${PROJECT_BINARY_DIR}/source.sh
        DESTINATION ${GROUP_INSTALL_DIR})
install(FILES ${PROJECT_BINARY_DIR}/libs_conf_${TAG}.cmake
        DESTINATION ${GROUP_INSTALL_DIR}
        RENAME libs_conf.cmake
        )
