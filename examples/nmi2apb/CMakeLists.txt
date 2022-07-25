cmake_minimum_required(VERSION 3.8)
project(env VERSION 1.0)

find_package(verilator HINTS $ENV{VERILATOR_ROOT} ${VERILATOR_ROOT})
if (NOT verilator_FOUND)
  message(FATAL_ERROR "Verilator was not found. Either install it, or set the VERILATOR_ROOT environment variable")
endif()

# set(CMAKE_VERBOSE_MAKEFILE ON)
# set(CMAKE_FIND_DEBUG_MODE 1)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS "")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(USE_VERILATOR True)


# SystemC dependencies
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# Find SystemC using SystemC's CMake integration
set(CMAKE_PREFIX_PATH "${SYSTEMC}")
find_package(SystemCLanguage CONFIG REQUIRED)
if (NOT SystemCLanguage_FOUND)
    message(FATAL_ERROR "SystemC not found")
endif()


set(SC_UVM_ENV_dir "$ENV{SC_UVM_ENV_HOME}")
set(CMAKE_PREFIX_PATH "$ENV{SC_UVM_ENV_HOME}")
find_package(SC_UVM_env CONFIG REQUIRED)
if (NOT SC_UVM_env_FOUND)
    message(FATAL_ERROR "SystemC not found")
endif()

list(APPEND EXTRA_LIBS "${SC_UVM_ENV_dir}/lib/libAPB_lib.a")
list(APPEND EXTRA_LIBS "${SC_UVM_ENV_dir}/lib/libNMI_lib.a")

list(APPEND EXTRA_INCLUDES "${SC_UVM_ENV_dir}/src/apb/inc")
list(APPEND EXTRA_INCLUDES "${SC_UVM_ENV_dir}/src/nmi/inc")


# set (CMAKE_CXX_STANDARD ${SystemC_CXX_STANDARD} CACHE STRING
#      "C++ standard to build all targets. Supported values are 98, 11, and 14.")
# set (CMAKE_CXX_STANDARD_REQUIRED ${SystemC_CXX_STANDARD_REQUIRED} CACHE BOOL
#      "The with CMAKE_CXX_STANDARD selected C++ standard is a requirement.")


 message(STATUS "Systemc Standard ${SystemC_CXX_STANDARD}")
 message(STATUS "CXX STANDARD ${CMAKE_CXX_STANDARD}")

if(USE_VERILATOR)
  list(APPEND EXTRA_INCLUDES "${VERILATOR_ROOT}/include")
endif()

add_subdirectory(rtl)

# Create a new executable target that will contain all your sources
add_executable(${PROJECT_NAME} 
                "${PROJECT_SOURCE_DIR}/src/sc_main.cpp"
                # ENV SOURCES
                "${PROJECT_SOURCE_DIR}/src/env.cpp"
                "${PROJECT_SOURCE_DIR}/src/scoreboard.cpp"
    )

# SCT Source file that needs to be compiled
set(SCT_DIR "$ENV{SCT_HOME}/components/common/sctcommon/")
set(SCT_SOURCES "${SCT_DIR}/sct_property.cpp")
target_sources(${PROJECT_NAME} 
                PRIVATE ${SCT_SOURCES}

                )

# Add SCV directory with libscv.so to path
target_link_directories(${PROJECT_NAME} 
                PRIVATE "$ENV{SCV}/objdir/src/scv/.libs"
                PRIVATE "$ENV{SYSTEMC_UVM}/lib-linux64"


                )

target_include_directories(${PROJECT_NAME} 
                PRIVATE "$ENV{SCV}/src"
                PRIVATE "$ENV{SYSTEMC_UVM}/include"
                PRIVATE "$ENV{FC4SC}/includes"
                PRIVATE "${SCT_DIR}"

                # ENV INCLUDES
                PRIVATE "inc"
                ${EXTRA_INCLUDES}
                )

target_link_libraries(${PROJECT_NAME} 
                PRIVATE rtl
                PRIVATE SystemC::systemc
                PRIVATE scv
                PRIVATE uvm-systemc 

                ${EXTRA_LIBS}
                )

