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

message(STATUS "G++: ${CMAKE_CXX_COMPILER}")
set(GCC_VERSION ${CMAKE_CXX_COMPILER_VERSION})

if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
    execute_process(
        COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
    if (NOT (GCC_VERSION VERSION_GREATER 7.0 OR GCC_VERSION VERSION_EQUAL 7.0))
        message(STATUS "${PROJECT_NAME} requires g++ 7.0 or greater.")

        set(GCC_VERSION 12.2.0)
        set(GCC_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/gcc-${GCC_VERSION})
        set(GCC_SRC_DIR ${PROJECT_BINARY_DIR}/src/gcc)

        if(CMAKE_SYSTEM MATCHES ${CMAKE_SYSTEM_MIC_CLUSTER})
            set(GCC_URL https://cernbox.cern.ch/remote.php/dav/public-files/jHtARNduUdYaQdM/gcc-12.2.0_centos7.tar.gz)
            ExternalProject_Add(gcc
                                URL ${GCC_URL}
                                PREFIX ${PROJECT_BINARY_DIR}
                                INSTALL_DIR ${GCC_INSTALL_DIR}
                                CONFIGURE_COMMAND ""
                                BUILD_COMMAND ""
                                INSTALL_COMMAND ${CMAKE_COMMAND} -E copy_directory ${GCC_SRC_DIR} ${GCC_INSTALL_DIR}
                                )


        else()
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

            message(STATUS "GCC_FLAGS: ${GCC_CFG_ARGS}")
            # string(REGEX REPLACE ";" " " GCC_CFG_ARGS "${GCC_CFG_ARGS}")
            # set(GCC_CFG_ARGS ${GCC_CFG_ARGS})
            message(STATUS "GCC_FLAGS: ${GCC_CFG_ARGS}")
            ExternalProject_Add(gcc
                                URL ${GCC_URL}
                                PREFIX ${PROJECT_BINARY_DIR}
                                INSTALL_DIR ${GCC_INSTALL_DIR}
                                CONFIGURE_COMMAND ${GCC_SRC_DIR}/configure --prefix=${GCC_INSTALL_DIR} ${GCC_CFG_ARGS}
                                )

            ExternalProject_Add_Step(gcc build_deps
                                    COMMAND  cd ${GCC_SRC_DIR} && ${GCC_SRC_DIR}/contrib/download_prerequisites && cd -
                                    COMMENT "Building prerequisites for GCC compilation"
                                    DEPENDEES download patch update
                                    DEPENDERS configure build install
                                    )
        endif()
    else()
        add_library(gcc INTERFACE)
    endif ()

elseif ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
    message(FATAL_ERROR "Clang is not supported at the moment")
else ()
    message(FATAL_ERROR "Other compilers than GNU are not supported at the moment")
endif ()
