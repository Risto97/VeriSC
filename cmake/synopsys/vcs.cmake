function(vcs_rtl OUT_LIB RTL_LIB)
    get_target_property(V_SOURCES ${RTL_LIB} INTERFACE_V_SOURCES)
    get_target_property(V_ARGS ${RTL_LIB} VERILOG_ARGS)

    list(GET V_SOURCES 0 TOP_V_FILE)
    get_filename_component(V_SOURCE_WO_EXT ${TOP_V_FILE} NAME_WE)

    if(V_ARGS STREQUAL "V_ARGS-NOTFOUND")
        set(V_ARGS "")
    endif()

    find_file(libs_conf_cmake "libs_conf.cmake"
        PATHS "$ENV{SC_UVM_ENV_HOME}/vcs" NO_DEFAULT_PATH
        NO_CACHE
        )
    include(${libs_conf_cmake})

    set(COSIM_DFLT_MAP_FILE ${CMAKE_CURRENT_BINARY_DIR}/cosim_defaults.map) # TODO make configurable per RTL_LIB
    add_custom_target(vlogan_copy_default_portmap_${RTL_LIB}
        COMMAND ${CMAKE_COMMAND} -E copy
            $ENV{SC_UVM_ENV_HOME}/cmake/synopsys/cosim_defaults.map
            ${COSIM_DFLT_MAP_FILE}
        )

    set(VCS_SC_WRAPPER_DIR "${PROJECT_BINARY_DIR}/vcsc_wrap")
    set(VCS_SC_WRAPPER_SRC "${VCS_SC_WRAPPER_DIR}/sysc/${V_SOURCE_WO_EXT}/${V_SOURCE_WO_EXT}.cpp")
    set(VCS_SC_WRAPPER_INC "${VCS_SC_WRAPPER_DIR}/sysc/include")

    set(VCS_LD_LIBRARY_PATH "${VG_GNU_PACKAGE}/gcc-9.2.0_64/lib64:${VG_GNU_PACKAGE}/gcc-9.2.0_64/lib")
    set(VCS_PATH_MOD "${VG_GNU_PACKAGE}/gcc-9.2.0_64/bin/:$ENV{VG_GNU_PACKAGE}/binutils-2.33.1_64/bin/:$ENV{PATH}}")

    set(VG_GNU_PACKAGE "${VCS_HOME}/gnu/linux64/")
    set(VCS_PATH_ENV "${VCS_HOME}/bin:${VCS_HOME}/gnu/linux64/binutils-2.33.1_64/bin/:$ENV{PATH}")
    set(VCS_LIB_PATH "${VCS_HOME}/gnu/linux64/gcc-9.2.0_64/lib64/:${VCS_HOME}/gnu/linux64/gcc-9.2.0_64/lib/")
    set(VCS_CXX "${VG_GNU_PACKAGE}/gcc-9.2.0_64/bin/g++")
    set(VCS_CC "${VG_GNU_PACKAGE}/gcc-9.2.0_64/bin/gcc")

    add_custom_command(OUTPUT ${VCS_SC_WRAPPER_SRC}
        COMMAND 
            ${CMAKE_COMMAND} -E env "VCS_HOME=${VCS_HOME}"
            ${CMAKE_COMMAND} -E env "VG_GNU_PACKAGE=${VG_GNU_PACKAGE}"
            ${CMAKE_COMMAND} -E env "PATH=${VCS_PATH_ENV}"
            ${CMAKE_COMMAND} -E env "LD_LIBRARY_PATH=${VCS_LIB_PATH}"
                vlogan -full64 -nc -sverilog
                    -cpp ${VCS_CXX}
                    -sc_model ${V_SOURCE_WO_EXT} ${TOP_V_FILE}
        DEPENDS ${V_SOURCES}
        COMMENT "Generating VCS SystemC wrapper for Verilog module"
    )

    add_custom_target(vlog_${OUT_LIB} 
        DEPENDS ${VCS_SC_WRAPPER_SRC}
        )
    set_property(TARGET vlog_${OUT_LIB}
        APPEND PROPERTY ADDITIONAL_CLEAN_FILES ${PROJECT_BINARY_DIR}/csrc ${PROJECT_BINARY_DIR}/AN.DB
        )
    add_dependencies(vlog_${OUT_LIB} vlogan_copy_default_portmap_${RTL_LIB})

endfunction()


function(vcs_tb SC_LIB RTL_LIBS)
    find_file(libs_conf_cmake "libs_conf.cmake"
        PATHS "$ENV{SC_UVM_ENV_HOME}/vcs" NO_DEFAULT_PATH
        NO_CACHE
        )
    include(${libs_conf_cmake})

    list(GET V_SOURCES 0 TOP_V_FILE)
    get_filename_component(V_SOURCE_WO_EXT ${TOP_V_FILE} NAME_WE)

    get_target_property(ENV_SOURCES ${SC_LIB} INTERFACE_SOURCES)
    get_target_property(ENV_INC_DIR ${SC_LIB} INTERFACE_INCLUDE_DIRECTORIES)
    get_target_property(ENV_COMP_OPT ${SC_LIB} INTERFACE_COMPILE_OPTIONS)

    find_library(sc_uvm_lib uvm-systemc
        PATHS "${SYSTEMC_UVM_HOME_VCS}/*" NO_DEFAULT_PATH 
        NO_CACHE
        )
    find_library(uvc_lib uvc
        PATHS "${UVC_HOME_VCS}/*" NO_DEFAULT_PATH 
        NO_CACHE
        )

    list(APPEND VCS_EXTRA_LIBS 
        ${sc_uvm_lib}
        ${uvc_lib}
        )

    list(APPEND VCS_EXTRA_INCLUDES 
        "${SYSTEMC_HOME_VCS}/include"
        "${SYSTEMC_UVM_HOME_VCS}/include"
        "${FC4SC_HOME_VCS}/includes"

        "${UVC_HOME_VCS}/include"
        )
    list(APPEND VCS_EXTRA_INCLUDES ${ENV_INC_DIR})

    string (REPLACE ";" " -I" CFLAGS_INC_DIRS "${VCS_EXTRA_INCLUDES}")
    set(CFLAGS_INC_DIRS "-I${CFLAGS_INC_DIRS}")

    foreach(lib ${VCS_EXTRA_LIBS})
        cmake_path(GET lib PARENT_PATH lib_dir)
        list(APPEND VCS_LDFLAGS "-Wl,-rpath,${lib_dir}")
    endforeach()
    list(APPEND VCS_LDFLAGS ${VCS_EXTRA_LIBS})
    string (REPLACE ";" " " VCS_LDFLAGS "${VCS_LDFLAGS}")

    set(VCS_LD_LIBRARY_PATH "${VG_GNU_PACKAGE}/gcc-9.2.0_64/lib64:${VG_GNU_PACKAGE}/gcc-9.2.0_64/lib")
    set(VCS_PATH_MOD "${VG_GNU_PACKAGE}/gcc-9.2.0_64/bin/:$ENV{VG_GNU_PACKAGE}/binutils-2.33.1_64/bin/:$ENV{PATH}}")

    set(VG_GNU_PACKAGE "${VCS_HOME}/gnu/linux64/")
    set(VCS_PATH_ENV "${VCS_HOME}/bin:${VCS_HOME}/gnu/linux64/binutils-2.33.1_64/bin/:$ENV{PATH}")
    set(VCS_LIB_PATH "${VCS_HOME}/gnu/linux64/gcc-9.2.0_64/lib64/:${VCS_HOME}/gnu/linux64/gcc-9.2.0_64/lib/")
    set(VCS_CXX "${VG_GNU_PACKAGE}/gcc-9.2.0_64/bin/g++")
    set(VCS_CC "${VG_GNU_PACKAGE}/gcc-9.2.0_64/bin/gcc")

    foreach(lib ${RTL_LIBS})
        list(APPEND VLOG_TARGETS vlog_${lib})
    endforeach()

    add_custom_target(vcscan  VERBATIM
        COMMAND 
            ${CMAKE_COMMAND} -E env "VCS_HOME=${VCS_HOME}"
            ${CMAKE_COMMAND} -E env "VG_GNU_PACKAGE=${VG_GNU_PACKAGE}"
            ${CMAKE_COMMAND} -E env "PATH=${VCS_PATH_ENV}"
            ${CMAKE_COMMAND} -E env "LD_LIBRARY_PATH=${VCS_LIB_PATH}"
                syscan -full64
                    -cpp ${VCS_CXX} -cc ${VCS_CC}
                    -sysc=scv20
                    ${ENV_SOURCES}
                    -CFLAGS "${CFLAGS_INC_DIRS} -O3"
        DEPENDS ${VLOG_TARGETS}
    )

    list(POP_FRONT V_SOURCES V_SOURCES_NO_TOP)
    add_custom_target(vcsrun  VERBATIM
        COMMAND 
        ${CMAKE_COMMAND} -E env "VCS_HOME=${VCS_HOME}"
        ${CMAKE_COMMAND} -E env "VG_GNU_PACKAGE=${VG_GNU_PACKAGE}"
        ${CMAKE_COMMAND} -E env "PATH=${VCS_PATH_ENV}"
        ${CMAKE_COMMAND} -E env "LD_LIBRARY_PATH=${VCS_LIB_PATH}"
            vcs -full64 -nc -j16
                -cpp ${VCS_CXX} -cc ${VCS_CC}
                -sysc=scv20 sc_main
                -timescale=1ns/1ps
                -LDFLAGS "${VCS_LDFLAGS}" #${V_SOURCES} TODO IS IT NEEDED?
                +vcs+dumpvars+wave.vcd
                -o vcs_tb
        DEPENDS vcscan
        )
    set_property(TARGET vcsrun
        APPEND PROPERTY ADDITIONAL_CLEAN_FILES vcs_tb
        )

    add_custom_target(vrun
        ${COMMAND} vcs_tb
        DEPENDS vcsrun
        )

    set_property(TARGET vrun
        APPEND PROPERTY ADDITIONAL_CLEAN_FILES wave.vcd
        )
endfunction()
