function(verilator)
    find_file(libs_conf_cmake "libs_conf.cmake"
        PATHS "$ENV{SC_UVM_ENV_HOME}/open" NO_DEFAULT_PATH
        NO_CACHE
        )
    include(${libs_conf_cmake})

    string(REPLACE "_env" "_rtl" RTL_LIB ${PROJECT_NAME})
    get_target_property(IP_V_SOURCES ${RTL_LIB} INTERFACE_V_SOURCES) # TODO CHANGE
    set(V_SOURCES ${IP_V_SOURCES})

    message(DEBUG "RTL_LIB included: ${RTL_LIB}")
    message(DEBUG "V_SOURCES from RTL_LIB ${V_SOURCES}")

    set(TAG OPEN)
    find_library(uvc_lib uvc
        PATHS "${UVC_HOME_${TAG}}/*"
        NO_DEFAULT_PATH
        NO_CACHE
        )
    find_library(systemc_lib_sci systemc
        PATHS "${SYSTEMC_HOME_${TAG}}/*" NO_DEFAULT_PATH
        NO_CACHE
        )
    find_library(sc_uvm_lib uvm-systemc
        PATHS "${SYSTEMC_UVM_HOME_${TAG}}/*" NO_DEFAULT_PATH
        NO_CACHE
        )
    find_library(scv_lib scv
        PATHS "${SCV_HOME_${TAG}}/*" NO_DEFAULT_PATH
        NO_CACHE
        )

    list(APPEND EXTRA_LIBS 
                ${systemc_lib_sci}
                ${sc_uvm_lib}
                ${scv_lib}
                ${uvc_lib}
                )

    list(APPEND EXTRA_INCLUDES 
        "${SYSTEMC_HOME_${TAG}}/include"
        "${SYSTEMC_UVM_HOME_${TAG}}/include"
        "${SCV_HOME_${TAG}}/include"
        "${FC4SC_HOME_${TAG}}/includes"
        "${SCT_HOME_${TAG}}/components/common/sctcommon/"

        "${UVC_HOME_${TAG}}/include" 
        )

    set(VERILATOR_ROOT ${VERILATOR_HOME_${TAG}})
    find_package(verilator HINTS ${VERILATOR_ROOT})
    if (NOT verilator_FOUND)
      message(FATAL_ERROR "Verilator was not found. Either install it, or set the VERILATOR_ROOT environment variable")
    endif()

# specify the C++ standard
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED True)

# SystemC dependencies
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)

    list(GET V_SOURCES 0 TOP_V_FILE)
    get_filename_component(V_SOURCE_WO_EXT ${TOP_V_FILE} NAME_WE)

    add_library(verilated_lib EXCLUDE_FROM_ALL)
    verilate(verilated_lib SYSTEMC
            SOURCES ${V_SOURCES}
            PREFIX ${V_SOURCE_WO_EXT}
            VERILATOR_ARGS --pins-sc-uint --trace -no-trace-params --trace-structs -Wno-fatal
            )

    target_include_directories(verilated_lib PRIVATE "${SYSTEMC_HOME_${TAG}}/include")
    target_link_libraries(verilated_lib PRIVATE ${systemc_lib})
    target_link_options(verilated_lib PRIVATE -pthread)

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
    add_custom_target(run
        ${COMMAND} 
        ${CMAKE_COMMAND} -E env "LD_LIBRARY_PATH=${GLIB_DIR}:$ENV{LD_LIBRARY_PATH}"
            verilator_tb
        DEPENDS verilator_tb)
               
endfunction()
