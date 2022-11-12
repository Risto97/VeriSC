
string(REPLACE "_env" "_rtl" RTL_LIB ${PROJECT_NAME})
get_target_property(IP_V_SOURCES ${RTL_LIB} INTERFACE_V_SOURCES) # TODO CHANGE
set(V_SOURCES ${IP_V_SOURCES})

message(DEBUG "RTL_LIB included: ${RTL_LIB}")
message(DEBUG "V_SOURCES from RTL_LIB ${V_SOURCES}")

set(CMAKE_FIND_DEBUG_MODE ON)
find_library(systemc_lib systemc PATHS "$ENV{SYSTEMC}/*" NO_DEFAULT_PATH)
set(CMAKE_FIND_DEBUG_MODE OFF)
find_library(sc_uvm_lib uvm-systemc PATHS "$ENV{SYSTEMC_UVM}/*" NO_DEFAULT_PATH)
find_library(scv_lib scv PATHS "$ENV{SCV}/objdir/src/scv/.libs/" NO_DEFAULT_PATH)

message(STATUS "SYSTEMC:: ${systemc_lib}")

list(APPEND EXTRA_LIBS 
            ${systemc_lib}
            ${sc_uvm_lib}
            ${scv_lib}
            ${SC_UVM_env_libs}
            )

list(APPEND EXTRA_INCLUDES 
            "$ENV{SYSTEMC}/include"
            "$ENV{SYSTEMC_UVM}/include"
            "$ENV{FC4SC}/includes"
            "$ENV{SCT_HOME}/components/common/sctcommon/"

            ${SC_UVM_env_include_dirs}
            )

find_package(verilator HINTS $ENV{VERILATOR_ROOT} ${VERILATOR_ROOT})
if (NOT verilator_FOUND)
  message(FATAL_ERROR "Verilator was not found. Either install it, or set the VERILATOR_ROOT environment variable")
endif()

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# SystemC dependencies
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# Find SystemC using SystemC's CMake integration
# set(CMAKE_PREFIX_PATH "$ENV{SYSTEMC}" "$ENV{SYSTEMC}/build")
# find_package(SystemCLanguage CONFIG REQUIRED)
# if (NOT SystemCLanguage_FOUND)
#     message(FATAL_ERROR "SystemC not found")
# endif()

list(GET V_SOURCES 0 TOP_V_FILE)
get_filename_component(V_SOURCE_WO_EXT ${TOP_V_FILE} NAME_WE)

add_library(verilated_lib EXCLUDE_FROM_ALL)
verilate(verilated_lib SYSTEMC
        SOURCES ${V_SOURCES}
        PREFIX ${V_SOURCE_WO_EXT}
        VERILATOR_ARGS --pins-sc-uint --trace -no-trace-params --trace-structs -Wno-fatal
        )

target_include_directories(verilated_lib PRIVATE "$ENV{SYSTEMC}/include")
target_link_libraries(verilated_lib PRIVATE ${systemc_lib})
target_link_options(verilated_lib PRIVATE -pthread)
# verilator_link_systemc(verilated_lib)

get_target_property(ENV_SOURCES ${PROJECT_NAME} INTERFACE_SOURCES)
get_target_property(ENV_INC_DIR ${PROJECT_NAME} INTERFACE_INCLUDE_DIRECTORIES)
get_target_property(ENV_COMP_OPT ${PROJECT_NAME} INTERFACE_COMPILE_OPTIONS)

message(DEBUG "INTERACE SOURCES ${ENV_SOURCES}")
message(DEBUG "INTERFACE INC_DIR ${ENV_INC_DIR}")
message(DEBUG "ENV COMP OPT ${ENV_COMP_OPT}")

add_executable(verilator_tb EXCLUDE_FROM_ALL
                ${ENV_SOURCES})

target_compile_options(verilator_tb PRIVATE
                    -DVERILATOR=1
                    ${ENV_COMP_OPT}
                    )
target_link_options(verilator_tb PRIVATE -pthread)

target_include_directories(verilator_tb PRIVATE
                           ${ENV_INC_DIR}
                           ${EXTRA_INCLUDES}
                           )

target_link_libraries(verilator_tb PRIVATE
                      verilated_lib  
                      ${EXTRA_LIBS}
                    )
           
