
if(NOT DEFINED SYSTEMC_HOME)
    ExternalProject_Add(systemc
        URL https://www.accellera.org/images/downloads/standards/systemc/systemc-${SYSTEMC_VERSION}.tar.gz
        PREFIX ${PROJECT_BINARY_DIR}
        INSTALL_DIR ${SYSTEMC_INSTALL_DIR}
        CMAKE_ARGS
            -DCMAKE_INSTALL_PREFIX=${SYSTEMC_INSTALL_DIR}
            -DCMAKE_INSTALL_LIBDIR=lib
            -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}
            -DENABLE_PHASE_CALLBACKS_TRACING=OFF
            -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
            -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
        )
else()
    add_library(systemc INTERFACE)
endif()
