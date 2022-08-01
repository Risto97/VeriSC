cmake_minimum_required(VERSION 3.8)
project(env VERSION 1.0)

set(CMAKE_VERBOSE_MAKEFILE ON)
# set(CMAKE_FIND_DEBUG_MODE 1)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS "-DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(USE_VERILATOR True)

set(CMAKE_PREFIX_PATH "$ENV{SC_UVM_ENV_HOME}")
find_package(SC_UVM_env CONFIG REQUIRED)
if (NOT SC_UVM_env_FOUND)
    message(FATAL_ERROR "SystemC not found")
endif()

list(APPEND EXTRA_LIBS "${SC_UVM_env_libs}")
list(APPEND EXTRA_INCLUDES "${SC_UVM_env_include_dirs}")

add_subdirectory(rtl)

add_executable(${PROJECT_NAME} 
                "${PROJECT_SOURCE_DIR}/src/sc_main.cpp"
                "${PROJECT_SOURCE_DIR}/src/env.cpp"
                "${PROJECT_SOURCE_DIR}/src/scoreboard.cpp"
               )


target_include_directories(${PROJECT_NAME} 
                PRIVATE "${EXTRA_INCLUDES}"
                # ENV INCLUDES
                PRIVATE "inc"
                )

target_link_libraries(${PROJECT_NAME} 
                      PRIVATE rtl
                      PRIVATE ${EXTRA_LIBS}
                     )

