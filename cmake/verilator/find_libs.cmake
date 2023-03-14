function(find_libs libs)
    include("$ENV{VERISC_HOME}/verisc_config.cmake")
    if(EXISTS "$ENV{VERISC_HOME}/user_config.cmake")
        include("$ENV{VERISC_HOME}/user_config.cmake")
    endif()


    if(DEFINED SYSTEMC_HOME) # SystemC is a cmake package, so look for it in the preinstalled directory
        find_package(SystemCLanguage CONFIG REQUIRED
            PATHS ${SYSTEMC_HOME}/*
            NO_DEFAULT_PATH
            )
    else()
        find_package(SystemCLanguage CONFIG REQUIRED
            PATHS $ENV{VERISC_HOME}/open/*
            NO_DEFAULT_PATH
            )
    endif()
    if(SystemCLanguage_FOUND)
        list(APPEND libs_found SystemC::systemc)
    endif()

    find_package(fc4sc CONFIG
        PATHS $ENV{VERISC_HOME}/open/*
        NO_DEFAULT_PATH
        )
    if(fc4sc_FOUND)
        list(APPEND libs_found fc4sc::fc4sc)
    endif()

    set(libs_to_find uvm-systemc scv uvc)

    foreach(l ${libs_to_find})
        find_package(${l} CONFIG
            PATHS $ENV{VERISC_HOME}/open/*
            NO_DEFAULT_PATH
            )
        if(${${l}_FOUND})
            list(APPEND libs_found ${l}::shared)
        endif()
    endforeach()

    set(${libs} ${libs_found} PARENT_SCOPE)
endfunction()
