
# Synopsys VCS

string(REPLACE "_env" "_rtl" RTL_LIB ${PROJECT_NAME})
get_target_property(IP_V_SOURCES ${RTL_LIB} INTERFACE_V_SOURCES)
set(V_SOURCES ${IP_V_SOURCES})

message(DEBUG "RTL_LIB included: ${RTL_LIB}")
message(DEBUG "V_SOURCES from RTL_LIB ${V_SOURCES}")

list(GET V_SOURCES 0 TOP_V_FILE)
get_filename_component(V_SOURCE_WO_EXT ${TOP_V_FILE} NAME_WE)

get_target_property(CPP_SOURCES ${PROJECT_NAME} SOURCES)
get_target_property(ENV_SOURCES ${PROJECT_NAME} INTERFACE_SOURCES)
get_target_property(ENV_INC_DIR ${PROJECT_NAME} INTERFACE_INCLUDE_DIRECTORIES)
get_target_property(ENV_COMP_OPT ${PROJECT_NAME} INTERFACE_COMPILE_OPTIONS)

find_library(sc_uvm_lib uvm-systemc PATHS "$ENV{VCS_SYSTEMC_UVM}/*" NO_DEFAULT_PATH)
find_library(sc_uvm_env_lib SC_UVM_env PATHS "$ENV{VCS_SC_UVM_ENV_HOME}/*" NO_DEFAULT_PATH)

list(APPEND VCS_EXTRA_LIBS 
            ${sc_uvm_lib}
            ${sc_uvm_env_lib}
            )

list(APPEND VCS_EXTRA_INCLUDES 
            "$ENV{VCS_SYSTEMC}/include"
            "$ENV{VCS_SYSTEMC_UVM}/include"
            "$ENV{VCS_FC4SC}/includes"

            "$ENV{VCS_SC_UVM_ENV_HOME}/src/apb/inc" 
            "$ENV{VCS_SC_UVM_ENV_HOME}/src/nmi/inc"
            "$ENV{VCS_SC_UVM_ENV_HOME}/src/i2c/inc"
            "$ENV{VCS_SC_UVM_ENV_HOME}/src/common/inc"
            ) #TODO Fix SC_UVM_HOME includes
list(APPEND VCS_EXTRA_INCLUDES ${ENV_INC_DIR})

string (REPLACE ";" " -I" CFLAGS_INC_DIRS "${VCS_EXTRA_INCLUDES}")
set(CFLAGS_INC_DIRS "-I${CFLAGS_INC_DIRS}")
message(STATUS "VCS Include Directories: ${CFLAGS_INC_DIRS}")

string (REPLACE ";" " " LDLIBS "${LDLIBS}")

message(STATUS "LIBS: ${LIBS}")
message(STATUS "Lodaer libS: ${LDLIBS}")

set(VCS_SC_WRAPPER_DIR "${PROJECT_BINARY_DIR}/vcsc_wrap")
set(VCS_SC_WRAPPER_SRC "${VCS_SC_WRAPPER_DIR}/sysc/${V_SOURCE_WO_EXT}/${V_SOURCE_WO_EXT}.cpp")
set(VCS_SC_WRAPPER_INC "${VCS_SC_WRAPPER_DIR}/sysc/include")
# set(VLOGAN_GEN_OBJ "csrc/sysc/${V_SOURCE_WO_EXT}/${V_SOURCE_WO_EXT}.o")

message(STATUS "USE VCS in RTL")
set(CMAKE_VERBOSE_MAKEFILE ON)
# add_library(${PROJECT_NAME} INTERFACE)

add_custom_command(OUTPUT ${VCS_SC_WRAPPER_SRC}
    COMMAND vlogan -full64 -nc -sverilog -sc_model ${V_SOURCE_WO_EXT} ${TOP_V_FILE} # -Mdir=${VCS_SC_WRAPPER_DIR}
                   DEPENDS ${V_SOURCES}
                   COMMENT "Generating VCS SystemC wrapper for Verilog module"
                   )
add_custom_target(vlog 
                  DEPENDS ${VCS_SC_WRAPPER_SRC})

add_dependencies(vlog vlogan_copy_default_portmap)
set_property(TARGET vlog
    APPEND PROPERTY ADDITIONAL_CLEAN_FILES ${PROJECT_BINARY_DIR}/csrc ${PROJECT_BINARY_DIR}/AN.DB
    )

# set_property(SOURCE ${VCS_SC_WRAPPER_SRC} PROPERTY GENERATED 1)
add_custom_target(vcscan  VERBATIM
    COMMAND syscan -full64 -cpp $ENV{VCS_CXX} -cc $ENV{VCS_CC} -sysc=scv20 ${ENV_SOURCES} -CFLAGS "${CFLAGS_INC_DIRS} -O3"
                  DEPENDS vlog
                  )

list(POP_FRONT V_SOURCES V_SOURCES_NO_TOP)
message(STATUS "V SRC POPPED ${V_SOURCES}")
add_custom_target(vcsrun  VERBATIM
    COMMAND vcs -full64 -nc -j16 -cpp $ENV{VCS_CXX} -cc $ENV{VCS_CC} -sysc=scv20 sc_main -LDFLAGS "${LDLIBS}" ${V_SOURCES} +vcs+dumpvars+wave.vcd
                  DEPENDS vcscan)

# target_include_directories(vcsrun 
#                            PRIVATE "$ENV{VCS_HOME}/include"
#                            PRIVATE "$ENV{VCS_HOME}/lib"
#                            PRIVATE "$ENV{VCS_HOME}/include/scv-2.0"
#                            PRIVATE "$ENV{VCS_HOME}/include/systemc233"
#                            PRIVATE "$ENV{VCS_HOME}/include/cosim/bf"
#                            PRIVATE "$ENV{SYSTEMC_HOME}/include"
#                            PUBLIC ${VCS_SC_WRAPPER_INC}
#                            )
# target_compile_options(vcsrun PRIVATE
#         -DVCSSYSTEM=1
#         -MMD
#         -m64
#     )
#

message(STATUS "CUrrent binary dir ${CMAKE_CURRENT_BINARY_DIR}")
add_custom_target(vlogan_copy_default_portmap
                    COMMAND ${CMAKE_COMMAND} -E copy
                    $ENV{SC_UVM_ENV_HOME}/cmake/synopsys/cosim_defaults.map
                    ${CMAKE_CURRENT_BINARY_DIR}/cosim_defaults.map)
