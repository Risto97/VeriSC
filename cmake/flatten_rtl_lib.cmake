include("$ENV{SC_UVM_ENV_HOME}/cmake/safe_get_target_property.cmake")

function(flatten_rtl_lib OUT_LIB)
    # Get dependencies
    get_target_property(LINK_LIBS ${OUT_LIB} INTERFACE_LINK_LIBRARIES)

    # Recursively flatten the libraries and copy or append properties
    if(LINK_LIBS STREQUAL LINK_LIBS-NOTFOUND)
        set(LINK_LIBS "")
    else()
        foreach(lib ${LINK_LIBS})
            flatten_rtl_lib(${lib})
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

    set(PROPERTY_APPEND_LIST "VERILATOR_CFG_FILES;VERILATOR_ARGS;VERILOG_DEFS;VCS_ARGS;RDL_FILES") # APPEND HERE from ARGV TODO
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
