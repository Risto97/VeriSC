cmake_minimum_required(VERSION 3.25)
project(veriSC VERSION 0.0.3)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    message(WARNING "Please define installation directonry by calling cmake with -DCMAKE_INSTALL_PREFIX=<installation-path>
    Using default directory ${PROJECT_BINARY_DIR}")
    set(CMAKE_INSTALL_PREFIX ${PROJECT_BINARY_DIR})
endif()

if(DEFINED VERISC_CFG)
    include(${VERISC_CFG})
endif()
include(${PROJECT_SOURCE_DIR}/verisc_config.cmake)

include(ExternalProject)

if(DEFINED GCC_HOME)
    set(CXX_COMPILER ${GCC_HOME}/bin/g++)
    set(C_COMPILER ${GCC_HOME}/bin/gcc)
else()
    set(CXX_COMPILER ${CMAKE_CXX_COMPILER})
    set(C_COMPILER ${CMAKE_C_COMPILER})
endif()
ExternalProject_Add(gcc_install
            DOWNLOAD_COMMAND ""
            PREFIX ${PROJECT_BINARY_DIR}
            SOURCE_DIR ${PROJECT_SOURCE_DIR}/gcc
            BUILD_ALWAYS 1
            CMAKE_ARGS
                -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
                -DGCC_VERSION=${GCC_VERSION}
                -DCMAKE_VERBOSE_MAKEFILE=${CMAKE_VERBOSE_MAKEFILE}
                -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}
                -DCMAKE_CXX_COMPILER=${CXX_COMPILER}
                -DCMAKE_C_COMPILER=${C_COMPILER}
            )


set(INSTALL_DIRECTORY ${CMAKE_INSTALL_PREFIX}/open)
ExternalProject_Add(tools_install
            DOWNLOAD_COMMAND ""
            PREFIX ${PROJECT_BINARY_DIR}
            SOURCE_DIR "${PROJECT_SOURCE_DIR}/tools"
            BUILD_ALWAYS 1
            CMAKE_ARGS
                -DCMAKE_INSTALL_PREFIX=${INSTALL_DIRECTORY}
                -DVERISC_INSTALL_DIR=${CMAKE_INSTALL_PREFIX}
                -DCMAKE_VERBOSE_MAKEFILE=${CMAKE_VERBOSE_MAKEFILE}

                -DVERISC_CFG=${VERISC_CFG}

                -DSYSTEMC_VERSION=${SYSTEMC_VERSION}
                -DSCV_VERSION=${SCV_VERSION}
                -DUVM-SYSTEMC_VERSION=${UVM-SYSTEMC_VERSION}
                -DVERILATOR_VERSION=${VERILATOR_VERSION}
                -DFC4SC_VERSION=${FC4SC_VERSION}
                -DICSC_COMPILER_VERSION=${ICSC_COMPILER_VERSION}

                -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}
            DEPENDS gcc_install
            )



if((NOT UVM-SYSTEMC_VERSION STREQUAL "FALSE") AND (NOT SCV_VERSION STREQUAL "FALSE") AND (NOT UVC_VERSION STREQUAL "FALSE"))
    ExternalProject_Add(uvc
                DOWNLOAD_COMMAND ""
                PREFIX ${PROJECT_BINARY_DIR}
                SOURCE_DIR "${PROJECT_SOURCE_DIR}/uvc"
                BUILD_ALWAYS 1
                CMAKE_ARGS
                    -DCMAKE_INSTALL_PREFIX=${INSTALL_DIRECTORY}/uvc
                    -DLIBS_CFG_DIR=${INSTALL_DIRECTORY}
                    -DCMAKE_INSTALL_INCLUDEDIR=include
                    -DVERISC_INSTALL_DIR=${CMAKE_INSTALL_PREFIX}
                    -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}
                    -DCMAKE_VERBOSE_MAKEFILE=${CMAKE_VERBOSE_MAKEFILE}
                DEPENDS gcc_install tools_install
                )
endif()

# if(NOT DEFINED VCS_HOME AND NOT DEFINED ENV{VCS_HOME})
    # message(WARNING "VCS libraries will not be compiled, VCS will not be used, run cmake with -DVCS_HOME=<path to vcs> or set environment variable VCS_HOME")
if(1)
    message(WARNING "VCS IS DISABLED AT THE MOMENT")
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
include(CMakePackageConfigHelpers)

install(
    FILES 
    "${PROJECT_SOURCE_DIR}/cmake/safe_get_target_property.cmake"
    "${PROJECT_SOURCE_DIR}/cmake/rtllib.cmake"
    "${PROJECT_SOURCE_DIR}/cmake/CMakeGraphVizOptions.cmake"
    DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake"
  )
install(
    FILES "${PROJECT_SOURCE_DIR}/scripts/graphviz_shorten_path.py"
    DESTINATION "${CMAKE_INSTALL_PREFIX}/scripts"
  )
install(
    FILES 
    "${PROJECT_SOURCE_DIR}/cmake/verilator/verilator.cmake" 
    "${PROJECT_SOURCE_DIR}/cmake/verilator/find_libs.cmake"
    DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake/verilator"
  )
install(
    FILES "${PROJECT_SOURCE_DIR}/cmake/verilator/verilator_ext_prj.cmake"
    DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake/verilator"
    RENAME CMakeLists.txt
  )

write_basic_package_version_file(
    "${PROJECT_NAME}ConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
    )
configure_file(
    ${PROJECT_SOURCE_DIR}/veriSCConfig.cmake.in
    ${PROJECT_BINARY_DIR}/veriSCConfig.cmake
    @ONLY
    )
configure_file(
    ${PROJECT_SOURCE_DIR}/deps_cfg.cmake.in
    ${PROJECT_BINARY_DIR}/deps_cfg.cmake
    @ONLY
    )


install(FILES 
    "${PROJECT_BINARY_DIR}/veriSCConfig.cmake" 
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    DESTINATION lib/cmake/${PROJECT_NAME}
    )

install(FILES 
    "${PROJECT_BINARY_DIR}/deps_cfg.cmake" 
    DESTINATION ${CMAKE_INSTALL_PREFIX}
    )
