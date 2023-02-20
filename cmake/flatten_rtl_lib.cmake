include("$ENV{SC_UVM_ENV_HOME}/cmake/safe_get_target_property.cmake")

function(flatten_rtl_lib OUT_LIB)
    get_target_property(FLATTENED ${OUT_LIB} FLATTENED)
    if(NOT FLATTENED)
        recursive_flatten_rtl_lib(${OUT_LIB})
        set_property(TARGET ${OUT_LIB} PROPERTY FLATTENED 1)
    else()
        message(FATAL_ERROR "DON'T Flatten lib more than 1 time ${OUT_LIB}")
    endif()

endfunction()

function(recursive_flatten_rtl_lib OUT_LIB)
    # Get dependencies
    get_target_property(LINK_LIBS ${OUT_LIB} INTERFACE_LINK_LIBRARIES)

    # Recursively flatten the libraries and copy or append properties
    if(LINK_LIBS STREQUAL LINK_LIBS-NOTFOUND)
        set(LINK_LIBS "")
    else()
        foreach(lib ${LINK_LIBS})
            recursive_flatten_rtl_lib(${lib})
        endforeach()
    endif()

    # Append SOURCES
    foreach(lib ${LINK_LIBS})
        get_interface_sources(srcs ${lib})
        target_sources(${OUT_LIB} PUBLIC ${srcs})
    endforeach()

    # Append Include Directories
    safe_get_target_property(src_dir ${OUT_LIB} SOURCE_DIR "FATAL")
    target_include_directories(${OUT_LIB} INTERFACE ${src_dir})
    foreach(lib ${LINK_LIBS})
        safe_get_target_property(inc_dirs ${lib} INTERFACE_INCLUDE_DIRECTORIES "FATAL")
        target_include_directories(${OUT_LIB} INTERFACE ${inc_dirs})
    endforeach()

    set(PROPERTY_APPEND_LIST "VERILATOR_CFG_FILES;VERILATOR_ARGS;VERILOG_DEFS;VCS_ARGS;RDL_FILES;DOCS_FILES;DOCS_LIB;GRAPHIC_FILES") # APPEND HERE from ARGV TODO
    foreach(lib ${LINK_LIBS})
        foreach(property_name ${PROPERTY_APPEND_LIST})
            safe_get_target_property(property_val ${lib} ${property_name} "")
            set_property(TARGET ${OUT_LIB}
                APPEND PROPERTY
                    ${property_name} ${property_val}
                    )
        endforeach()
    endforeach()
endfunction()

function(is_flat OUT_VAR LIB)
    get_target_property(FLATTENED ${LIB} FLATTENED)
    if(FLATTENED STREQUAL "FLATTENED-NOTFOUND")
        set(${OUT_VAR} 0 PARENT_SCOPE)
    else()
        set(${OUT_VAR} ${FLATTENED} PARENT_SCOPE)
    endif()

endfunction()
