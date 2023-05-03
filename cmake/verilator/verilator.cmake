set(__VERISC_REL_ROOT "${CMAKE_CURRENT_LIST_DIR}/../../")

include("${__VERISC_REL_ROOT}/verisc_config.cmake")
if(EXISTS "${__VERISC_REL_ROOT}/user_config.cmake")
    include("${__VERISC_REL_ROOT}/user_config.cmake")
endif()
if(NOT DEFINED VERILATOR_HOME)
    set(VERILATOR_HOME ${__VERISC_REL_ROOT}/open/verilator-${VERILATOR_VERSION})
endif()

function(verilate_rtl LIB)
    set(OPTIONS "COVERAGE;TRACE;TRACE_FST;SYSTEMC;TRACE_STRUCTS;EXCLUDE_FROM_ALL")
    set(ONE_PARAM_ARGS "PREFIX;TOP_MODULE;THREADS;TRACE_THREADS;DIRECTORY")
    set(MULTI_PARAM_ARGS "VERILATOR_ARGS;OPT_SLOW;OPT_FAST;OPT_GLOBAL")

    cmake_parse_arguments(VERILATE "${OPTIONS}"
        "${ONE_PARAM_ARGS}"
        "${MULTI_PARAM_ARGS}"
        ${ARGN})

    include("${CMAKE_CURRENT_FUNCTION_LIST_DIR}/../rtllib.cmake")

    get_target_property(BINARY_DIR ${LIB} BINARY_DIR)

    get_rtl_target_property(INTERFACE_INCLUDE_DIRECTORIES ${LIB} INTERFACE_INCLUDE_DIRECTORIES)
    get_rtl_target_property(INCLUDE_DIRECTORIES ${LIB} INCLUDE_DIRECTORIES)
    if(INTERFACE_INCLUDE_DIRECTORIES)
        set(INCLUDE_DIRS_ARG INCLUDE_DIRS ${INTERFACE_INCLUDE_DIRECTORIES} ${INCLUDE_DIRECTORIES})
    endif()

    get_rtl_target_sources(V_FILES ${LIB} SOURCES)

    if(VERILATE_TOP_MODULE)
        set(TOP_MODULE_ARG TOP_MODULE ${VERILATE_TOP_MODULE})
    else()
        list(GET V_FILES -1 LAST_SOURCE)
        get_filename_component(TOP_MOD ${LAST_SOURCE} NAME_WE)
        set(TOP_MODULE_ARG TOP_MODULE ${TOP_MOD})
    endif()

    if(VERILATE_EXCLUDE_FROM_ALL)
        set(VERILATE_EXCLUDE_FROM_ALL "EXCLUDE_FROM_ALL")
    else()
        set(VERILATE_EXCLUDE_FROM_ALL "")
    endif()

    get_rtl_target_property(LIB_VERILATOR_ARGS ${LIB} VERILATOR_ARGS)
    set(VERILATOR_ARGS VERILATOR_ARGS ${VERILATE_VERILATOR_ARGS} ${LIB_VERILATOR_ARGS})

    list(REMOVE_DUPLICATES V_FILES)

    if(NOT VERILATE_PREFIX)
        set(VERILATE_PREFIX)
    endif()

    verilate_verisc(${LIB} ${VERILATE_EXCLUDE_FROM_ALL}
        SOURCES ${V_FILES}
        ${VERILATOR_ARGS}

        ${ARGN}
        ${INCLUDE_DIRS_ARG}
        ${TOP_MODULE_ARG}
        )

    add_dependencies(${LIB}_vlt ${LIB})

endfunction()

function(verilate_verisc TARGET)
    set(OPTIONS "COVERAGE;TRACE;TRACE_FST;SYSTEMC;TRACE_STRUCTS;EXCLUDE_FROM_ALL")
    set(ONE_PARAM_ARGS "PREFIX;TOP_MODULE;THREADS;TRACE_THREADS;DIRECTORY")
    set(MULTI_PARAM_ARGS "SOURCES;VERILATOR_ARGS;INCLUDE_DIRS;OPT_SLOW;OPT_FAST;OPT_GLOBAL")
    cmake_parse_arguments(VERILATE 
        "${OPTIONS}"
        "${ONE_PARAM_ARGS}"
        "${MULTI_PARAM_ARGS}"
        ${ARGN})


    if (NOT VERILATE_SOURCES)
        message(FATAL_ERROR "Need at least one source")
    endif()

    if(NOT VERILATE_TOP_MODULE)
        list(GET VERILATE_SOURCES 0 FIRST_SOURCE)
        get_filename_component(TOP_MODULE ${FIRST_SOURCE} NAME_WE)
    else()
        set(TOP_MODULE ${VERILATE_TOP_MODULE})
    endif()

    if(VERILATE_EXCLUDE_FROM_ALL)
        set(VERILATE_EXCLUDE_FROM_ALL "EXCLUDE_FROM_ALL")
    else()
        set(VERILATE_EXCLUDE_FROM_ALL "")
    endif()

    if(NOT VERILATE_DIRECTORY)
        set(VERILATE_DIRECTORY "${PROJECT_BINARY_DIR}/${TARGET}_vlt/verilate")
    endif()

    if(VERILATE_PREFIX)
        set(TOP_MODULE ${VERILATE_PREFIX})
    else()
        if(VERILATE_TOP_MODULE)
            set(VERILATE_PREFIX "V${VERILATE_TOP_MODULE}")
        endif()
    endif()


    foreach(param ${MULTI_PARAM_ARGS})
        string(REPLACE ";" "|" VERILATE_${param} "${VERILATE_${param}}")
    endforeach()

    foreach(param ${OPTIONS} ${ONE_PARAM_ARGS} ${MULTI_PARAM_ARGS})
        if(VERILATE_${param})
            list(APPEND EXT_PRJ_ARGS "-DVERILATE_${param}=${VERILATE_${param}}")
            list(APPEND ARGUMENTS_LIST ${param})
        endif()
    endforeach()
    string(REPLACE ";" "|" ARGUMENTS_LIST "${ARGUMENTS_LIST}")

    if(CMAKE_CXX_STANDARD)
        set(ARG_CMAKE_CXX_STANDARD "-DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}")
    endif()

      include(ExternalProject)
      ExternalProject_Add(${TARGET}_vlt
          DOWNLOAD_COMMAND ""
          SOURCE_DIR "${CMAKE_CURRENT_FUNCTION_LIST_DIR}"
          PREFIX ${PROJECT_BINARY_DIR}/${TARGET}_vlt
          BINARY_DIR ${PROJECT_BINARY_DIR}/${TARGET}_vlt
          LIST_SEPARATOR |
          BUILD_ALWAYS 1

          CMAKE_ARGS
              ${ARG_CMAKE_CXX_STANDARD}
              -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
              -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
              -DCMAKE_VERBOSE_MAKEFILE=${CMAKE_VERBOSE_MAKEFILE}

              -DTARGET=${TARGET}
              -DARGUMENTS_LIST=${ARGUMENTS_LIST}
              ${EXT_PRJ_ARGS}
              -DVERILATOR_ROOT=${VERILATOR_HOME}

          INSTALL_COMMAND ""
          DEPENDS ${TARGET}
          EXCLUDE_FROM_ALL 1
          ) 

    set(VLT_STATIC_LIB "${PROJECT_BINARY_DIR}/${TARGET}_vlt/lib${TARGET}.a")
    set(INC_DIR ${VERILATE_DIRECTORY})

    add_library(tmp_${TARGET} STATIC IMPORTED)
    set_target_properties(tmp_${TARGET} PROPERTIES IMPORTED_LOCATION ${VLT_STATIC_LIB})
    
    target_include_directories(tmp_${TARGET} INTERFACE ${INC_DIR})
    target_include_directories(tmp_${TARGET} INTERFACE
        "${VERILATOR_HOME}/include"
        "${VERILATOR_HOME}/include/vltstd")
    
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)

    cmake_policy(SET CMP0079 NEW)

    add_library(${TARGET}_verilated INTERFACE)
    target_link_libraries(${TARGET}_verilated INTERFACE tmp_${TARGET} -pthread)
    set_property(TARGET ${TARGET} PROPERTY VLT_LIB ${TARGET}_verilated)
    add_dependencies(${TARGET}_verilated ${TARGET} ${TARGET}_vlt)

endfunction()

function(verilate_tb EXEC)
    set(multiValueArgs RTL_LIBS DEPS)
    set(options "")
    set(oneValueArgs "")
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    include("${__VERISC_REL_ROOT}/cmake/verilator/find_libs.cmake")
    find_libs(libs)
    if(NOT ${ARG_DEPS} STREQUAL "")
        set(libs ${ARG_DEPS})
    endif()

    target_compile_options(${EXEC} PRIVATE
                        -DVERILATOR=1
                        )

    if(NOT ARG_RTL_LIBS)
        message(FATAL_ERROR "No RTL Libs provided")
    endif()
    foreach(lib ${ARG_RTL_LIBS})
        get_target_property(VLT_LIB ${lib} VLT_LIB)
        list(APPEND VLT_LIBS ${VLT_LIB})
    endforeach()
    target_link_libraries(${EXEC} PRIVATE
        ${VLT_LIBS} 
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
