include("$ENV{SC_UVM_ENV_HOME}/cmake/flatten_rtl_lib.cmake")

function(verilate_rtl OUT_LIB RTL_LIB)
    get_interface_sources(V_SOURCES ${RTL_LIB})
    safe_get_target_property(VLT_CFG_FILES ${RTL_LIB} VERILATOR_CFG_FILES "")
    safe_get_target_property(VERILATOR_ARGS ${RTL_LIB} VERILATOR_ARGS "")
    safe_get_target_property(VERILOG_DEFS ${RTL_LIB} VERILOG_DEFS "")
    safe_get_target_property(INCLUDE_DIRS ${RTL_LIB} INTERFACE_INCLUDE_DIRECTORIES "")

    find_file(libs_conf_cmake "libs_conf.cmake"
        PATHS "$ENV{SC_UVM_ENV_HOME}/open" NO_DEFAULT_PATH
        NO_CACHE
        )
    set(TAG "OPEN")
    include(${libs_conf_cmake})
    find_library(systemc_lib systemc
        PATHS "${SYSTEMC_HOME_${TAG}}/*" NO_DEFAULT_PATH
        NO_CACHE
        )

    set(VERILATOR_ROOT ${VERILATOR_HOME_${TAG}})

    list(REVERSE V_SOURCES)
    list(GET V_SOURCES 0 TOP_V_FILE)
    get_filename_component(V_SOURCE_WO_EXT ${TOP_V_FILE} NAME_WE)
    set(TOP_MODULE ${V_SOURCE_WO_EXT})

    include(ExternalProject)

    string(REPLACE ";" "|" V_SOURCES "${V_SOURCES}")
    string(REPLACE ";" "|" INCLUDE_DIRS "${INCLUDE_DIRS}")
    string(REPLACE ";" "|" VERILOG_DEFS "${VERILOG_DEFS}")
    string(REPLACE ";" "|" VLT_CFG_FILES "${VLT_CFG_FILES}")
    string(REPLACE ";" "|" VERILATOR_ARGS "${VERILATOR_ARGS}")
    set(EXT_PRJ ${TOP_MODULE}_vlt)
    ExternalProject_Add(${EXT_PRJ}
        DOWNLOAD_COMMAND ""
        SOURCE_DIR "$ENV{SC_UVM_ENV_HOME}/cmake/verilator"
        PREFIX ${PROJECT_BINARY_DIR}/${EXT_PRJ}
        BINARY_DIR ${PROJECT_BINARY_DIR}/${EXT_PRJ}
        LIST_SEPARATOR |
        BUILD_ALWAYS 1

        CMAKE_ARGS
            -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}
            -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
            -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}

            -DV_SOURCES=${V_SOURCES}
            -DVLT_CFG_FILES=${VLT_CFG_FILES}
            -DTOP_MODULE=${TOP_MODULE}
            -DVERILATOR_ARGS=${VERILATOR_ARGS}
            -DVERILOG_DEFS=${VERILOG_DEFS}
            -DINCLUDE_DIRS=${INCLUDE_DIRS}

            -DVERILATOR_ROOT=${VERILATOR_ROOT}
            -DSYSTEMC_HOME=${SYSTEMC_HOME_${TAG}}
            -DSYSTEMC_LIB=${systemc_lib}

        INSTALL_COMMAND ""
        DEPENDS ${RTL_LIB}
        EXCLUDE_FROM_ALL 1
        ) 

    set(LIB_A "${PROJECT_BINARY_DIR}/${EXT_PRJ}/lib${TOP_MODULE}.a")
    set(INC_DIR "${PROJECT_BINARY_DIR}/${EXT_PRJ}/${EXT_PRJ}")
    
    add_library(tmp_${TOP_MODULE} STATIC IMPORTED)
    add_dependencies(tmp_${TOP_MODULE} ${EXT_PRJ})
    set_target_properties(tmp_${TOP_MODULE} PROPERTIES IMPORTED_LOCATION ${LIB_A})

    target_include_directories(${OUT_LIB} INTERFACE ${INC_DIR})
    target_include_directories(${OUT_LIB} INTERFACE
        "${VERILATOR_ROOT}/include"
        "${VERILATOR_ROOT}/include/vltstd")

    target_link_libraries(${OUT_LIB} INTERFACE tmp_${TOP_MODULE})

endfunction()

function(verilate_tb SC_LIB RTL_LIBS)

    find_file(libs_conf_cmake "libs_conf.cmake"
        PATHS "$ENV{SC_UVM_ENV_HOME}/open" NO_DEFAULT_PATH
        NO_CACHE
        )
    include(${libs_conf_cmake})

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

    get_target_property(ENV_SOURCES ${SC_LIB} INTERFACE_SOURCES)
    get_target_property(ENV_INC_DIR ${SC_LIB} INTERFACE_INCLUDE_DIRECTORIES)
    get_target_property(ENV_COMP_OPT ${SC_LIB} INTERFACE_COMPILE_OPTIONS)

    add_executable(verilator_tb EXCLUDE_FROM_ALL
        ${ENV_SOURCES}
        )
    add_dependencies(verilator_tb ${SC_LIB})

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
                          ${RTL_LIBS} 
                          ${EXTRA_LIBS}
                        )
    if(DEFINED LIBSTDCPP_DIR)    # IF non system GCC is used set RPATH for libstdc++
       set_target_properties(verilator_tb PROPERTIES
          BUILD_WITH_INSTALL_RPATH TRUE
          INSTALL_RPATH_USE_LINK_PATH TRUE
          INSTALL_RPATH "${LIBSTDCPP_DIR}:${INSTALL_RPATH}")
    endif()

    get_target_property(RUN_ARGS ${PROJECT_NAME} RUN_ARGS)
    if(RUN_ARGS STREQUAL "RUN_ARGS-NOTFOUND")
        set(RUN_ARGS "")
    endif()

    add_custom_target(run
        ${COMMAND} ./verilator_tb ${RUN_ARGS}
        DEPENDS verilator_tb)
               
endfunction()
