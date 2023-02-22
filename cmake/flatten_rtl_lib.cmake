include("$ENV{SC_UVM_ENV_HOME}/cmake/safe_get_target_property.cmake")

function(flat_rtl_lib TARGET OUTLIB)
    flatten_graph(${TARGET})

    if(NOT TARGET ${OUTLIB})
        add_library(${OUTLIB} INTERFACE)
    endif()

    get_property(FLAT_GRAPH TARGET ${TARGET} PROPERTY FLAT_GRAPH)
    set_property(TARGET ${OUTLIB} PROPERTY FLAT_GRAPH ${FLAT_GRAPH})

    # Merge INTERFACE_SOURCES and SOURCES and put them in SOURCES in OUTLIB
    get_merged_rtl_sources(SOURCES ${TARGET})
    set_property(TARGET ${OUTLIB} PROPERTY SOURCES ${SOURCES})

    # Append include directories, the PROJECT_SOURCE_DIR is also added as include directory
    set_property(TARGET ${OUTLIB} PROPERTY INTERFACE_INCLUDE_DIRECTORIES "") 
    foreach(lib ${FLAT_GRAPH})
        safe_get_target_property(src_dirs ${lib} SOURCE_DIR "FATAL")
        safe_get_target_property(inc_dirs ${lib} INTERFACE_INCLUDE_DIRECTORIES "")
        set_property(TARGET ${OUTLIB} APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${inc_dirs} ${src_dirs})
    endforeach()

    set(PROPERTY_APPEND_LIST "VERILATOR_CFG_FILES;VERILATOR_ARGS;VERILOG_DEFS;VCS_ARGS;RDL_FILES;DOCS_FILES;DOCS_LIB;GRAPHIC_FILES") # APPEND HERE from ARGV TODO
    foreach(prop_name ${PROPERTY_APPEND_LIST})
        set_property(TARGET ${OUTLIB} PROPERTY ${prop_name} "")
        foreach(lib ${FLAT_GRAPH})
            safe_get_target_property(prop_val ${lib} ${prop_name} "")
            list(REVERSE prop_val)
            set_property(TARGET ${OUTLIB} APPEND PROPERTY ${prop_name} ${prop_val})
        endforeach()
        get_target_property(OUTVAR ${OUTLIB} ${prop_name})
    endforeach()


endfunction()

function(__append_rm_list_unique NODE RM_EL) # Append if element not already in list
    get_target_property(RM_LIST ${NODE} __RM_LIST )

    list(FIND RM_LIST ${RM_EL} FOUND)
    if(FOUND EQUAL -1)
        set_property(TARGET ${NODE} APPEND PROPERTY __RM_LIST ${RM_EL})
    endif()
endfunction()

function(compare_lists L1 L2 RET)
    list(LENGTH L1 L1_LEN)
    list(LENGTH L2 L2_LEN)

    if(NOT (L1_LEN EQUAL L2_LEN))
        set(${RET} -1 PARENT_SCOPE)
        return()
    endif()

    foreach(el ${L1})
        list(FIND L2 ${el} FOUND)
        if(FOUND EQUAL -1)
            set(${RET} -1 PARENT_SCOPE)
            return()
        endif()
    endforeach()

    set(${RET} 1 PARENT_SCOPE)
endfunction()

function(__all_vertices_removed NODE RET)
    get_target_property(RM_LIST ${NODE} __RM_LIST)
    get_target_property(LINK_LIBS ${NODE} INTERFACE_LINK_LIBRARIES)
    if(RM_LIST STREQUAL "RM_LIST-NOTFOUND")
        set(RM_LIST "")
    endif()
    if(LINK_LIBS STREQUAL "LINK_LIBS-NOTFOUND")
        set(LINK_LIBS "")
    endif()

    compare_lists("${RM_LIST}" "${LINK_LIBS}" L_EQ)
    set(${RET} ${L_EQ} PARENT_SCOPE)
endfunction()

function(__flatten_graph_recursive NODE RET)
    __all_vertices_removed(${NODE} ALL_VERTICES_REMOVED)
    if(ALL_VERTICES_REMOVED EQUAL 1) # IF there is not DEPS on this node
        
        get_property(STACK GLOBAL PROPERTY __GLOBAL_STACK)
        list(FIND STACK ${NODE} ALREADY_ADDED)

        if(ALREADY_ADDED EQUAL -1)  # If the node is not in stack append it to stack
            list(APPEND STACK ${NODE})
            set_property(GLOBAL PROPERTY __GLOBAL_STACK ${STACK})
        endif()

        set(${RET} 1 PARENT_SCOPE)
        return()
    endif()

    get_target_property(LINK_LIBS ${NODE} INTERFACE_LINK_LIBRARIES)
    if(LINK_LIBS STREQUAL "LINK_LIBS-NOTFOUND") # Not needed
        set(LINK_LIBS "")
    endif()
    foreach(lib ${LINK_LIBS})
        __flatten_graph_recursive(${lib} LIB_ADDED)
        if(LIB_ADDED EQUAL 1)
            __append_rm_list_unique(${NODE} ${lib})
            get_target_property(_RM_LIST ${NODE} __RM_LIST )
        endif()
    endforeach()

    set(${RET} 0 PARENT_SCOPE)
endfunction()

function(flatten_graph NODE)

    set_property(GLOBAL PROPERTY __GLOBAL_STACK "")
    set(FOUND_ROOT -1)
    while(FOUND_ROOT EQUAL -1)
        __flatten_graph_recursive(${NODE} FOUND_ROOT)
        list(FIND STACK ${NODE} FOUND_ROOT)
        get_property(STACK GLOBAL PROPERTY __GLOBAL_STACK)
    endwhile()

    foreach(lib ${STACK}) # CLear __RM_LIST
        set_property(TARGET ${lib} PROPERTY __RM_LIST "")
    endforeach()
    set_property(TARGET ${NODE} PROPERTY FLAT_GRAPH ${STACK})

endfunction()

function(get_target_property_flatten OUT_VAR TARGET PROPERTY)

    get_target_property(DEPS ${TARGET} FLAT_GRAPH)
    if(ARGV3 OR (DEPS STREQUAL "DEPS-NOTFOUND"))
        flatten_graph(${TARGET})
    endif()
    get_target_property(DEPS ${TARGET} FLAT_GRAPH)

    set(OUT_LIST "")
    foreach(d ${DEPS})
        safe_get_target_property(PROP ${d} ${PROPERTY} "")
        list(REVERSE PROP)
        list(APPEND OUT_LIST ${PROP})
    endforeach()

    set(${OUT_VAR} ${OUT_LIST} PARENT_SCOPE)
endfunction()

function(get_merged_rtl_sources OUT_VAR TARGET)
    get_target_property(DEPS ${TARGET} FLAT_GRAPH)
    foreach(d ${DEPS})
        safe_get_target_property(INTERFACE_SOURCES ${d} INTERFACE_SOURCES "")
        safe_get_target_property(SOURCES ${d} SOURCES "")
        list(REVERSE SOURCES) # Only reverse SOURCE, INTERFACE_SOURCES get added in proper order when used sequentially TODO, reconsider changing order of file dependencies
        list(APPEND V_SOURCES ${SOURCES} ${INTERFACE_SOURCES})
    endforeach()

    set(${OUT_VAR} ${V_SOURCES} PARENT_SCOPE)
endfunction()
