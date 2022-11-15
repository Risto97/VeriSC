cmake_minimum_required(VERSION 3.20)
project(gcc_install)

include(ExternalProject)

if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
		cmake_policy(SET CMP0135 NEW)
endif()

set(CMAKE_SYSTEM_MIC_CLUSTER "Linux-3.10.0-1160.71.1.el7.x86_64")
message("-- CMAKE_SYSTEM:           ${CMAKE_SYSTEM}")

string (REGEX MATCH "\\.el[1-9]" os_version_suffix ${CMAKE_SYSTEM})
message("-- os_version_suffix:      ${os_version_suffix}")


set(GCC_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/gcc-${GCC_VERSION})
set(GCC_SRC_DIR ${PROJECT_BINARY_DIR}/src/gcc)
set(CXX_COMPILER ${GCC_INSTALL_DIR}/bin/g++)
set(C_COMPILER ${GCC_INSTALL_DIR}/bin/gcc)
if(DOWNLOAD_GCC_BINARY AND NOT COMPILE_GCC) # Download binaries instead of compile
    message(STATUS "Downloading GCC binary: ${GCC_VERSION}")

    set(GCC_URL https://cernbox.cern.ch/remote.php/dav/public-files/jHtARNduUdYaQdM/gcc-12.2.0_centos7.tar.gz)
    ExternalProject_Add(gcc
                        URL ${GCC_URL}
                        PREFIX ${PROJECT_BINARY_DIR}
                        SOURCE_DIR ${GCC_SRC_DIR}
                        INSTALL_DIR ${GCC_INSTALL_DIR}
                        CONFIGURE_COMMAND ""
                        BUILD_COMMAND ""
                        INSTALL_COMMAND ${CMAKE_COMMAND} -E copy_directory ${GCC_SRC_DIR} ${GCC_INSTALL_DIR}
                        )

elseif(NOT DOWNLOAD_GCC_BINARY AND COMPILE_GCC) # Compile GCC
    message(STATUS "Compiling GCC: ${GCC_VERSION}")

    set(GCC_URL https://ftp.gnu.org/gnu/gcc/gcc-${GCC_VERSION}/gcc-${GCC_VERSION}.tar.gz)

    list(APPEND GCC_CFG_ARGS 
        "--build=x86_64-linux-gnu"
        "--host=x86_64-linux-gnu"
        "--target=x86_64-linux-gnu"
        "--enable-checking=release"
        "--enable-languages=c,c++"
        "--enable-shared"
        "--enable-threads=posix"
        "--enable-__cxa_atexit"
        "--enable-clocale=gnu"
        "--disable-multilib"
        )

    ExternalProject_Add(gcc
                        URL ${GCC_URL}
                        PREFIX ${PROJECT_BINARY_DIR}
                        SOURCE_DIR ${GCC_SRC_DIR}
                        INSTALL_DIR ${GCC_INSTALL_DIR}
                        CONFIGURE_COMMAND ${GCC_SRC_DIR}/configure --prefix=${GCC_INSTALL_DIR} ${GCC_CFG_ARGS}
                        )

    ExternalProject_Add_Step(gcc build_deps
                            COMMAND  cd ${GCC_SRC_DIR} && ${GCC_SRC_DIR}/contrib/download_prerequisites && cd -
                            COMMENT "Building prerequisites for GCC compilation"
                            DEPENDEES download patch update
                            DEPENDERS configure build install
                            )

elseif(NOT DOWNLOAD_GCC_BINARY AND NOT COMPILE_GCC) # GCC on system is compatible
    message(STATUS "Using system GCC: ${CMAKE_CXX_COMPILER}")
    set(CXX_COMPILER ${CMAKE_CXX_COMPILER})
    set(C_COMPILER ${CMAKE_C_COMPILER})
    add_library(gcc INTERFACE)
endif()

set(TAG "OSCI")
configure_file(${PROJECT_SOURCE_DIR}/gcc_conf.cmake.in ${PROJECT_BINARY_DIR}/gcc_conf_${TAG}.cmake)

install(FILES ${PROJECT_BINARY_DIR}/gcc_conf_${TAG}.cmake
        DESTINATION ${CMAKE_INSTALL_PREFIX}/osci
        RENAME gcc_conf.cmake
        )
