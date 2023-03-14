cmake_minimum_required(VERSION 3.20)
project(gcc_install)

include(ExternalProject)

if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
		cmake_policy(SET CMP0135 NEW)
endif()

set(COMPILE_GCC FALSE)

list(FIND CMAKE_CXX_COMPILE_FEATURES "cxx_std_${CMAKE_CXX_STANDARD}" COMPILER_SUPPORTS_STD) # Check if std_17 is supported

if(COMPILER_SUPPORTS_STD EQUAL -1)
    set(COMPILE_GCC TRUE)
endif()


if(COMPILE_GCC)
    set(GCC_VERSION ${GCC_VERSION})
    set(GCC_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/gcc-${GCC_VERSION})
    set(GCC_SRC_DIR ${PROJECT_BINARY_DIR}/src/gcc)
    set(GCC_URL https://ftp.gnu.org/gnu/gcc/gcc-${GCC_VERSION}/gcc-${GCC_VERSION}.tar.gz)

    list(APPEND GCC_CFG_ARGS 
        "--build=${CMAKE_SYSTEM_PROCESSOR}-linux-gnu" # TODO check if this works for ARM and RISCV ????
        "--host=${CMAKE_SYSTEM_PROCESSOR}-linux-gnu"
        "--target=${CMAKE_SYSTEM_PROCESSOR}-linux-gnu"
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

    set(CMAKE_CXX_COMPILER_PATH ${GCC_INSTALL_DIR}/bin/g++)
    set(CMAKE_C_COMPILER_PATH ${GCC_INSTALL_DIR}/bin/gcc)
    set(CMAKE_AR_PATH ${GCC_INSTALL_DIR}/bin/gcc-ar)
    set(CMAKE_RANLIB_PATH ${GCC_INSTALL_DIR}/bin/gcc-ar)
    set(EXTERNAL_GCC 1)

else()
    set(GCC_VERSION ${CMAKE_CXX_COMPILER_VERSION})
    set(GCC_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/gcc-${GCC_VERSION})
    add_library(gcc INTERFACE)

    set(CMAKE_CXX_COMPILER_PATH ${CMAKE_CXX_COMPILER})
    set(CMAKE_C_COMPILER_PATH ${CMAKE_C_COMPILER})
    set(EXTERNAL_GCC 0)
endif()

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "gccConfigVersion.cmake"
    VERSION ${GCC_VERSION}
    COMPATIBILITY AnyNewerVersion
    )

configure_file(
    ${PROJECT_SOURCE_DIR}/gccConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/gccConfig.cmake
    @ONLY
    )
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/gccConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/gccConfigVersion.cmake"
    DESTINATION ${GCC_INSTALL_DIR}/lib/cmake/gcc
    )

