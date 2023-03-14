include("${CMAKE_CURRENT_LIST_DIR}/../flatten_rtl_lib.cmake")

function(verilate_rtl OUT_LIB RTL_LIB)
    get_interface_sources(V_SOURCES ${RTL_LIB})
    safe_get_target_property(VLT_CFG_FILES ${RTL_LIB} VERILATOR_CFG_FILES "")
    safe_get_target_property(VERILATOR_ARGS ${RTL_LIB} VERILATOR_ARGS "")
    safe_get_target_property(VERILOG_DEFS ${RTL_LIB} VERILOG_DEFS "")
    safe_get_target_property(INCLUDE_DIRS ${RTL_LIB} INTERFACE_INCLUDE_DIRECTORIES "")

    include("$ENV{VERISC_HOME}/verisc_config.cmake")
    if(EXISTS "$ENV{VERISC_HOME}/user_config.cmake")
        include("$ENV{VERISC_HOME}/user_config.cmake")
    endif()
    if(NOT DEFINED VERILATOR_HOME)
        set(VERILATOR_HOME $ENV{VERISC_HOME}/open/verilator-${VERILATOR_VERSION})
    endif()

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
        SOURCE_DIR "$ENV{VERISC_HOME}/cmake/verilator"
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
            -DVLT_ARGS=${VERILATOR_ARGS}
            -DVERILOG_DEFS=${VERILOG_DEFS}
            -DINCLUDE_DIRS=${INCLUDE_DIRS}

            -DVERILATOR_ROOT=${VERILATOR_HOME}

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
        "${VERILATOR_HOME}/include"
        "${VERILATOR_HOME}/include/vltstd")

    target_link_libraries(${OUT_LIB} INTERFACE tmp_${TOP_MODULE})

endfunction()

function(verilate_tb EXEC)
    set(multiValueArgs RTL_LIBS DEPS)
    set(options "")
    set(oneValueArgs "")
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    include("$ENV{VERISC_HOME}/cmake/verilator/find_libs.cmake")
    find_libs(libs)
    if(NOT ${ARG_DEPS} STREQUAL "")
        set(libs ${ARG_DEPS})
    endif()

    target_compile_options(${EXEC} PRIVATE
                        -DVERILATOR=1
                        )

    target_link_libraries(${EXEC} PRIVATE
        ${ARG_RTL_LIBS} 
        ${libs}
        )

    if(DEFINED CXX_LIBDIR)    # IF non system GCC is used set RPATH for libstdc++
        set_target_properties(${EXEC} PROPERTIES
            BUILD_WITH_INSTALL_RPATH TRUE
            INSTALL_RPATH_USE_LINK_PATH TRUE
            INSTALL_RPATH "${CXX_LIBDIR}:${INSTALL_RPATH}")
        endif()

    get_target_property(RUN_ARGS ${PROJECT_NAME} RUN_ARGS)
    if(RUN_ARGS STREQUAL "RUN_ARGS-NOTFOUND")
        set(RUN_ARGS "")
    endif()

    add_custom_target(run
        ${COMMAND} ./${EXEC} ${RUN_ARGS}
        DEPENDS ${EXEC})
               
endfunction()
