set(__VERISC_REL_ROOT "${CMAKE_CURRENT_LIST_DIR}/../../")
function(find_libs libs)

    include("${__VERISC_REL_ROOT}/deps_cfg.cmake")

    if(DEFINED SYSTEMC_HOME) # SystemC is a cmake package, so look for it in the preinstalled directory
        find_package(SystemCLanguage CONFIG REQUIRED
            PATHS ${SYSTEMC_HOME}/*
            NO_DEFAULT_PATH
            )
    else()
        find_package(SystemCLanguage CONFIG REQUIRED
            PATHS ${__VERISC_REL_ROOT}/open/*
            NO_DEFAULT_PATH
            )
    endif()
    if(SystemCLanguage_FOUND)
        list(APPEND libs_found SystemC::systemc)
    endif()

    if(NOT FC4SC_VERSION STREQUAL "FALSE")
        find_package(fc4sc CONFIG
            PATHS ${__VERISC_REL_ROOT}/open/*
            NO_DEFAULT_PATH
            )
        if(fc4sc_FOUND)
            list(APPEND libs_found fc4sc::fc4sc)
        endif()
    endif()

    if(NOT UVM-SYSTEMC_VERSION STREQUAL "FALSE")
        list(APPEND libs_to_find uvm-systemc)
    endif()

    if(NOT SCV_VERSION STREQUAL "FALSE")
        list(APPEND libs_to_find scv)
    endif()

    if(NOT UVC_VERSION STREQUAL "FALSE")
        list(APPEND libs_to_find uvc)
    endif()

    foreach(l ${libs_to_find})
        find_package(${l} CONFIG
            PATHS ${__VERISC_REL_ROOT}/open/*
            NO_DEFAULT_PATH
            )
        if(${${l}_FOUND})
            list(APPEND libs_found ${l}::shared)
        endif()
    endforeach()

    set(${libs} ${libs_found} PARENT_SCOPE)
endfunction()
