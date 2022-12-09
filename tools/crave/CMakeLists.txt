cmake_minimum_required(VERSION 3.20)
project(crave_install)

include(ExternalProject)

if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
		cmake_policy(SET CMP0135 NEW)
endif()

set(GROUP_INSTALL_DIR ${CMAKE_INSTALL_PREFIX})
# set(CMAKE_CXX_STANDARD ${CMAKE_CXX_STANDARD})

set(ZLIB_VERSION 1.2.13)
set(BOOST_VERSION 1.80.0) # 1.55.0
# set(GLOG_VERSION 0.6.0)

# set(BOOLECTOR_VERSION 3.2.2)
# set(BTOR2TOOLS_VERSION master)
# set(CRYPTOMINISAT_VERSION 5.11.4)
# set(LINGELING_VERSION master)
# set(CADICAL_VERSION 1.5.3)

set(CUDD_VERSION 3.0.0)
set(Z3_VERSION 4.8.4) 

set(CRAVE_VERSION accellera-contribution)

message("SYSTEMC_HOME: ${SYSTEMC_HOME} ()*)(*!#)(!*@#)(!@*#)(@!#*)")
message("SYSTEMC_HOME: ${SYSTEMC_HOME} ()*)(*!#)(!*@#)(!@*#)(@!#*)")
message("SYSTEMC_HOME: ${SYSTEMC_HOME} ()*)(*!#)(!*@#)(!@*#)(@!#*)")
message("SYSTEMC_HOME: ${SYSTEMC_HOME} ()*)(*!#)(!*@#)(!@*#)(@!#*)")
message("SYSTEMC_HOME: ${SYSTEMC_HOME} ()*)(*!#)(!*@#)(!@*#)(@!#*)")
message("SYSTEMC_HOME: ${SYSTEMC_HOME} ()*)(*!#)(!*@#)(!@*#)(@!#*)")
if(NOT DEFINED SYSTEMC_HOME)
    message(FATAL_ERROR "Please define SYSTEMC_HOME variable, exiting, remove this message to try and autofind: ${CMAKE_CURRENT_LIST_FILE}:${CMAKE_CURRENT_LIST_LINE}")
endif()

# set(ZLIB_INSTALL_DIR ${GROUP_INSTALL_DIR}/zlib-${ZLIB_VERSION})
# ExternalProject_Add(zlib
#     GIT_REPOSITORY https://github.com/madler/zlib.git
#     GIT_TAG v${ZLIB_VERSION}
#     PREFIX ${PROJECT_BINARY_DIR}
#     INSTALL_DIR ${ZLIB_INSTALL_DIR}
#     CMAKE_ARGS
#         -DCMAKE_INSTALL_PREFIX=${ZLIB_INSTALL_DIR}
#         -DCMAKE_INSTALL_LIBDIR=lib
#         -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}
#         -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
#         -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
#     )
#
set(BOOST_INSTALL_DIR_SHARED ${GROUP_INSTALL_DIR}/boost_so-${BOOST_VERSION}) # TODO REVIEW THIS
set(BOOST_INSTALL_DIR_STATIC ${GROUP_INSTALL_DIR}/boost_a-${BOOST_VERSION})
set(BOOST_SOURCE_DIR ${PROJECT_BINARY_DIR}/src/boost)
ExternalProject_Add(boost
    GIT_REPOSITORY https://github.com/boostorg/boost.git
    GIT_TAG boost-${BOOST_VERSION}
    PREFIX ${PROJECT_BINARY_DIR}
    BUILD_IN_SOURCE TRUE
    UPDATE_COMMAND ${BOOST_SOURCE_DIR}/bootstrap.sh
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ${BOOST_SOURCE_DIR}/b2 --prefix=${BOOST_INSTALL_DIR_STATIC} --with-system --with-filesystem -link=static -j16 install
    INSTALL_COMMAND ${BOOST_SOURCE_DIR}/b2 --prefix=${BOOST_INSTALL_DIR_SHARED} --with-system --with-filesystem -link=shared -j16 install
    )

# set(GLOG_INSTALL_DIR ${GROUP_INSTALL_DIR}/glog-${GLOG_VERSION})
# ExternalProject_Add(glog
#     GIT_REPOSITORY https://github.com/google/glog.git
#     GIT_TAG v${GLOG_VERSION}
#     PREFIX ${PROJECT_BINARY_DIR}
#     INSTALL_DIR ${GLOG_INSTALL_DIR}
#     CMAKE_ARGS
#     -DCMAKE_INSTALL_PREFIX=${GLOG_INSTALL_DIR}
#         -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}
#         -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
#         -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
#     )
#

# set(CRYPTOMINISAT_INSTALL_DIR ${GROUP_INSTALL_DIR}/cryptominisat-${CRYPTOMINISAT_VERSION})
# set(CRYPTOMINISAT_SOURCE_DIR ${PROJECT_BINARY_DIR}/src/cryptominisat )
# ExternalProject_Add(cryptominisat
#     GIT_REPOSITORY https://github.com/msoos/cryptominisat.git
#     GIT_TAG ${CRYPTOMINISAT_VERSION}
#     PREFIX ${PROJECT_BINARY_DIR}
#     INSTALL_DIR ${CRYPTOMINISAT_INSTALL_DIR}
#     LIST_SEPARATOR |
#     CMAKE_ARGS
#         -DCMAKE_PREFIX_PATH="${BOOST_INSTALL_DIR_STATIC}|${ZLIB_INSTALL_DIR}"
#         -DCMAKE_INSTALL_PREFIX=${CRYPTOMINISAT_INSTALL_DIR}
#         -DCMAKE_FIND_LIBRARY_SUFFIXES=.a
#         -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}
#         -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
#         -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
#         -DCMAKE_DISABLE_FIND_PACKAGE_PythonInterp=TRUE
#         -DCMAKE_DISABLE_FIND_PACKAGE_PythonLibs=TRUE
#         -DONLY_SIMPLE=ON
#         -DBUILD_SHARED_LIBS=OFF
#     DEPENDS boost zlib
#     )
#
# set(BTOR2TOOLS_INSTALL_DIR ${GROUP_INSTALL_DIR}/btor2tools-${BTOR2TOOLS_VERSION})
# set(BTOR2TOOLS_SOURCE_DIR ${PROJECT_BINARY_DIR}/src/btor2tools )
# ExternalProject_Add(btor2tools
#     GIT_REPOSITORY https://github.com/Boolector/btor2tools.git
#     GIT_TAG ${BTOR2TOOLS_VERSION}
#     PREFIX ${PROJECT_BINARY_DIR}
#     INSTALL_DIR ${BTOR2TOOLS_INSTALL_DIR}
#     CMAKE_ARGS
#     -DCMAKE_PREFIX_PATH=${GROUP_INSTALL_DIR}
#     -DBUILD_SHARED_LIBS=OFF
#     -DCMAKE_INSTALL_PREFIX=${BTOR2TOOLS_INSTALL_DIR}
#         -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}
#         -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
#         -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
#     DEPENDS boost
#     )
# ExternalProject_Add_Step(btor2tools post_install
#     COMMAND mkdir -p ${BTOR2TOOLS_INSTALL_DIR}/include/btor2parser/ && cp ${BTOR2TOOLS_INSTALL_DIR}/include/btor2parser.h ${BTOR2TOOLS_INSTALL_DIR}/include/btor2parser/
#     COMMENT "Copy file to expected path by boolector"
#     DEPENDEES install 
#     )

# set(LINGELING_INSTALL_DIR ${GROUP_INSTALL_DIR}/lingeling-${LINGELING_VERSION})
# set(LINGELING_SOURCE_DIR ${PROJECT_BINARY_DIR}/src/lingeling )
# ExternalProject_Add(lingeling
#     GIT_REPOSITORY https://github.com/arminbiere/lingeling.git
#     GIT_TAG ${LINGELING_VERSION}
#     PREFIX ${PROJECT_BINARY_DIR}
#     BUILD_IN_SOURCE TRUE
#     INSTALL_DIR ${LINGELING_INSTALL_DIR}
#     CONFIGURE_COMMAND ./configure.sh -fPIC --static
#     BUILD_COMMAND ${MAKE}
#     INSTALL_COMMAND mkdir -p ${LINGELING_INSTALL_DIR}/lib &&
#                     mkdir -p ${LINGELING_INSTALL_DIR}/include && 
#                     cp ${LINGELING_SOURCE_DIR}/liblgl.a ${LINGELING_INSTALL_DIR}/lib &&
#                     cp ${LINGELING_SOURCE_DIR}/lglib.h ${LINGELING_INSTALL_DIR}/include
#
#     )

# set(CADICAL_INSTALL_DIR ${GROUP_INSTALL_DIR}/cadical-${CADICAL_VERSION})
# set(CADICAL_SOURCE_DIR ${PROJECT_BINARY_DIR}/src/cadical )
# ExternalProject_Add(cadical
#     GIT_REPOSITORY https://github.com/arminbiere/cadical.git
#     GIT_TAG rel-${CADICAL_VERSION}
#     PREFIX ${PROJECT_BINARY_DIR}
#     BUILD_IN_SOURCE TRUE
#     INSTALL_DIR ${CADICAL_INSTALL_DIR}
#     CONFIGURE_COMMAND ./configure -fPIC -static
#     BUILD_COMMAND ${MAKE}
#     INSTALL_COMMAND mkdir -p ${CADICAL_INSTALL_DIR}/lib &&
#                     mkdir -p ${CADICAL_INSTALL_DIR}/include && 
#                     cp ${CADICAL_SOURCE_DIR}/build/libcadical.a ${CADICAL_INSTALL_DIR}/lib &&
#                     cp ${CADICAL_SOURCE_DIR}/src/ccadical.h ${CADICAL_INSTALL_DIR}/include
#
#     )
#
#
# set(BOOLECTOR_INSTALL_DIR ${GROUP_INSTALL_DIR}/boolector-${BOOLECTOR_VERSION})
# set(BOOLECTOR_SOURCE_DIR ${PROJECT_BINARY_DIR}/src/boolector )
# ExternalProject_Add(boolector
#     GIT_REPOSITORY https://github.com/Boolector/boolector.git
#     GIT_TAG ${BOOLECTOR_VERSION}
#     PREFIX ${PROJECT_BINARY_DIR}
#     INSTALL_DIR ${BOOLECTOR_INSTALL_DIR}
#     LIST_SEPARATOR |
#     CMAKE_ARGS
#         -DCMAKE_VERBOSE_MAKEFILE=TRUE
#         -DCMAKE_PREFIX_PATH=${BTOR2TOOLS_INSTALL_DIR}|${CRYPTOMINISAT_INSTALL_DIR}
#         -DCMAKE_INSTALL_PREFIX=${BOOLECTOR_INSTALL_DIR}
#         -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}
#         -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
#         -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
#     DEPENDS cryptominisat btor2tools glog lingeling cadical boost
#     )


set(CUDD_INSTALL_DIR ${GROUP_INSTALL_DIR}/cudd-${CUDD_VERSION})
set(CUDD_SOURCE_DIR ${PROJECT_BINARY_DIR}/src/cudd )
set(CUDD_BUILD_DIR ${PROJECT_BINARY_DIR}/src/cudd-build )
ExternalProject_Add(cudd         # TODO dont build docs
    GIT_REPOSITORY https://github.com/nbruns1/cudd.git
    GIT_TAG cudd-${CUDD_VERSION}
    PREFIX ${PROJECT_BINARY_DIR}
    BUILD_IN_SOURCE TRUE
    INSTALL_DIR ${CUDD_INSTALL_DIR}
    UPDATE_COMMAND touch configure.ac aclocal.m4 configure Makefile.am Makefile.in
    CONFIGURE_COMMAND ./configure -enable-obj --enable-dddmp --prefix=${CUDD_INSTALL_DIR}
    BUILD_COMMAND ${MAKE}
    INSTALL_COMMAND make install
    )
configure_file(
    ${PROJECT_SOURCE_DIR}/CUDDConfig.cmake.in
    ${CUDD_INSTALL_DIR}/lib/cmake/cudd/CUDDConfig.cmake
    )

set(Z3_INSTALL_DIR ${GROUP_INSTALL_DIR}/z3-${Z3_VERSION})
set(Z3_SOURCE_DIR ${PROJECT_BINARY_DIR}/src/z3 )
ExternalProject_Add(z3
    GIT_REPOSITORY https://github.com/Z3Prover/z3.git
    GIT_TAG z3-${Z3_VERSION}
    PREFIX ${PROJECT_BINARY_DIR}
    INSTALL_DIR ${Z3_INSTALL_DIR}
    LIST_SEPARATOR |
    CMAKE_ARGS
        -DBUILD_LIBZ3_SHARED=FALSE
        -DCMAKE_BUILD_TYPE=Release
        -DCMAKE_INSTALL_PREFIX=${Z3_INSTALL_DIR}
        -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}
        -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
        -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
    )

# set(SYSTEMC_HOME "/mnt/ext/sc_uvm/open/systemc-2.3.3/")
# set(SYSTEMC_HOME "/mnt/ext/sc_uvm/open/systemc-2.3.3/")
set(CMAKE_PREFIX_PATH_ALT 
   ${BOOST_INSTALL_DIR_STATIC}
   ${SYSTEMC_HOME}
   # ${GLOG_INSTALL_DIR}
   ${CUDD_INSTALL_DIR}
   ${Z3_INSTALL_DIR}
   )

string(REPLACE ";" "|" CMAKE_PREFIX_PATH_ALT_SEP "${CMAKE_PREFIX_PATH_ALT}")
message("PATHS: ${CMAKE_PREFIX_PATH_ALT_SEP}")

set(CRAVE_INSTALL_DIR ${GROUP_INSTALL_DIR}/crave-${CRAVE_VERSION})
set(CRAVE_SRC_DIR ${PROJECT_BINARY_DIR}/src/crave/crave )

ExternalProject_Add(crave
    GIT_REPOSITORY https://github.com/agra-uni-bremen/crave.git
    GIT_TAG ${CRAVE_VERSION}
    SOURCE_SUBDIR crave
    PREFIX ${PROJECT_BINARY_DIR}
    INSTALL_DIR ${CRAVE_INSTALL_DIR}
    LIST_SEPARATOR |
    CMAKE_ARGS
        -DCMAKE_VERBOSE_MAKEFILE=TRUE
        -DCMAKE_INSTALL_PREFIX=${CRAVE_INSTALL_DIR}
        -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}
        -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
        -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
        -DCMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH_ALT_SEP}
        -DCMAKE_FIND_LIBRARY_CUSTOM_LIB_SUFFIX=-linux64
        -DBUILD_SHARED_LIBS=TRUE

        -DmetaSMT_USE_Boolector=FALSE
        -DmetaSMT_USE_CVC4=FALSE
        -DmetaSMT_USE_STP=FALSE
        -DmetaSMT_USE_SWORD=FALSE
        -DmetaSMT_USE_YICES2=FALSE
        -DmetaSMT_USE_Z3=TRUE
        -DmetaSMT_USE_CUDD=TRUE
        -DmetaSMT_ENABLE_TESTS=OFF
        -DWITH_SYSTEMC=YES
        -DSYSTEMC_HOME=${SYSTEMC_HOME}
        -DGLOG_ROOT=do/not/use
    DEPENDS cudd z3 boost
#     # DEPENDS cryptominisat btor2tools glog lingeling cadical boost cudd
    )

message("Source: ${CRAVE_SRC_DIR}/../src/include/")
message("Destination: ${CRAVE_INSTALL_DIR}")
ExternalProject_Add_Step(crave install_crave2uvm
    COMMAND cp -r ${CRAVE_SRC_DIR}/../src/include/ ${CRAVE_INSTALL_DIR}
    COMMENT "Install additional CRAVE2UVM headers"
    DEPENDEES install
    )

configure_file(${PROJECT_SOURCE_DIR}/crave_conf.cmake.in ${PROJECT_BINARY_DIR}/crave_conf_${TAG}.cmake)

install(FILES ${PROJECT_BINARY_DIR}/crave_conf_${TAG}.cmake
        DESTINATION ${GROUP_INSTALL_DIR}
        RENAME crave_conf.cmake
        )
