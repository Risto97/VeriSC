if(NOT DEFINED VERILATOR_HOME)
    set(PO4A_INSTALL_DIR ${GROUP_INSTALL_DIR}/po4a-${PO4A_VERSION})
    set(PO4A_SRC_DIR ${PROJECT_BINARY_DIR}/src/po4a)
    ExternalProject_Add(po4a
        GIT_REPOSITORY https://github.com/mquinson/po4a.git
        GIT_TAG v${PO4A_VERSION}
        PREFIX ${PROJECT_BINARY_DIR}
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        INSTALL_COMMAND ${CMAKE_COMMAND} -E copy_directory ${PO4A_SRC_DIR} ${PO4A_INSTALL_DIR}
        )

    set(HELP2MAN_INSTALL_DIR ${GROUP_INSTALL_DIR}/help2man-${HELP2MAN_VERSION})
    set(HELP2MAN_SRC_DIR ${PROJECT_BINARY_DIR}/src/help2man)
    ExternalProject_Add(help2man
        URL https://mirrors.kernel.org/gnu/help2man/help2man-${HELP2MAN_VERSION}.tar.xz
        PREFIX ${PROJECT_BINARY_DIR}
        CONFIGURE_COMMAND cd ${HELP2MAN_SRC_DIR} && autoconf --output ${HELP2MAN_SRC_DIR}/configure  ${HELP2MAN_SRC_DIR}/configure.ac && ${HELP2MAN_SRC_DIR}/configure --prefix=${HELP2MAN_INSTALL_DIR} && cd -
        BUILD_COMMAND ""
        INSTALL_COMMAND cd ${HELP2MAN_SRC_DIR} && PATH=${PO4A_INSTALL_DIR}:$ENV{PATH} PERLLIB=${PO4A_INSTALL_DIR}/lib:$ENV{PERLLIB} $(MAKE) install && cd -
        DEPENDS po4a
        )

    set(VERILATOR_SRC_DIR ${PROJECT_BINARY_DIR}/src/verilator)
    ExternalProject_Add(verilator
        GIT_REPOSITORY https://github.com/verilator/verilator.git
        GIT_TAG v${VERILATOR_VERSION}
        PREFIX ${PROJECT_BINARY_DIR}
        PATCH_COMMAND patch -N ${VERILATOR_SRC_DIR}/verilator-config.cmake.in -i ${CMAKE_CURRENT_LIST_DIR}/verilator-config.patch || true
        CONFIGURE_COMMAND  autoconf --output ${VERILATOR_SRC_DIR}/configure ${VERILATOR_SRC_DIR}/configure.ac && ${VERILATOR_SRC_DIR}/configure CC=${CMAKE_C_COMPILER} CXX=${CMAKE_CXX_COMPILER} --prefix=${VERILATOR_INSTALL_DIR}
        BUILD_COMMAND PATH=${HELP2MAN_INSTALL_DIR}/bin:$ENV{PATH} $(MAKE)

        INSTALL_COMMAND make install && ${CMAKE_COMMAND} -E copy_directory ${VERILATOR_INSTALL_DIR}/share/verilator/include ${VERILATOR_INSTALL_DIR}/include
        DEPENDS help2man
        )

else()
    add_library(verilator INTERFACE)
endif()
