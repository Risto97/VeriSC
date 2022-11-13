cmake_minimum_required(VERSION 3.20)
project(tools_install)

include(ExternalProject)

set(GROUP_INSTALL_DIR ${PROJECT_BINARY_DIR}/installs/${CMAKE_CXX_COMPILER_VERSION})

set(SYSTEMC_VERSION 2.3.3)
set(SCV_VERSION 2.0.1)
set(UVM_VERSION 1.0-beta4)

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
    )

set(SCV_INSTALL_DIR ${GROUP_INSTALL_DIR}/scv-${SCV_VERSION})
ExternalProject_Add(scv
    URL https://www.accellera.org/images/downloads/standards/systemc/scv-${SCV_VERSION}.tar.gz
    PREFIX ${PROJECT_BINARY_DIR}
    CONFIGURE_COMMAND ${PROJECT_BINARY_DIR}/src/scv/configure --with-systemc=${SYSTEMC_INSTALL_DIR} --prefix=${SCV_INSTALL_DIR}
    DEPENDS systemc
    )

set(UVM_INSTALL_DIR ${GROUP_INSTALL_DIR}/uvm-systemc-${UVM_VERSION})
ExternalProject_Add(uvm-systemc
    URL https://www.accellera.org/images/downloads/standards/systemc/uvm-systemc-${UVM_VERSION}.tar.gz
    PREFIX ${PROJECT_BINARY_DIR}
    CONFIGURE_COMMAND ${PROJECT_BINARY_DIR}/src/uvm-systemc/configure --with-systemc=${SYSTEMC_INSTALL_DIR} --prefix=${UVM_INSTALL_DIR}
    DEPENDS systemc
    )
