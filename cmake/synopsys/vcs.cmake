
# Synopsys VCS

function(vcs)
    find_file(libs_conf_cmake "libs_conf.cmake"
        PATHS "$ENV{SC_UVM_ENV_HOME}/vcs" NO_DEFAULT_PATH)
    include(${libs_conf_cmake})

    string(REPLACE "_env" "_rtl" RTL_LIB ${PROJECT_NAME})
    get_target_property(IP_V_SOURCES ${RTL_LIB} INTERFACE_V_SOURCES)
    set(V_SOURCES ${IP_V_SOURCES})

    message(DEBUG "RTL_LIB included: ${RTL_LIB}")
    message(DEBUG "V_SOURCES from RTL_LIB ${V_SOURCES}")

    list(GET V_SOURCES 0 TOP_V_FILE)
    get_filename_component(V_SOURCE_WO_EXT ${TOP_V_FILE} NAME_WE)

    get_target_property(CPP_SOURCES ${PROJECT_NAME} SOURCES)
    get_target_property(ENV_SOURCES ${PROJECT_NAME} INTERFACE_SOURCES)
    get_target_property(ENV_INC_DIR ${PROJECT_NAME} INTERFACE_INCLUDE_DIRECTORIES)
    get_target_property(ENV_COMP_OPT ${PROJECT_NAME} INTERFACE_COMPILE_OPTIONS)

    find_library(sc_uvm_lib uvm-systemc PATHS "${SYSTEMC_UVM_HOME_VCS}/*" NO_DEFAULT_PATH)
    find_library(uvc_lib SC_UVM_env PATHS "${UVC_HOME_VCS}/*" NO_DEFAULT_PATH)

    list(APPEND VCS_EXTRA_LIBS 
                ${sc_uvm_lib}
                ${uvc_lib}
                )

    list(APPEND VCS_EXTRA_INCLUDES 
        "${SYSTEMC_HOME_VCS}/include"
        "${SYSTEMC_UVM_HOME_VCS}/include"
        "${FC4SC_HOME_VCS}/includes"

        "${UVC_HOME_VCS}/include"
        ) #TODO Fix SC_UVM_HOME includes
    list(APPEND VCS_EXTRA_INCLUDES ${ENV_INC_DIR})

    string (REPLACE ";" " -I" CFLAGS_INC_DIRS "${VCS_EXTRA_INCLUDES}")
    set(CFLAGS_INC_DIRS "-I${CFLAGS_INC_DIRS}")

    foreach(lib ${VCS_EXTRA_LIBS})
        cmake_path(GET lib PARENT_PATH lib_dir)
        list(APPEND VCS_LDFLAGS "-Wl,-rpath,${lib_dir}")
    endforeach()
    list(APPEND VCS_LDFLAGS ${VCS_EXTRA_LIBS})
    string (REPLACE ";" " " VCS_LDFLAGS "${VCS_LDFLAGS}")


    set(VCS_SC_WRAPPER_DIR "${PROJECT_BINARY_DIR}/vcsc_wrap")
    set(VCS_SC_WRAPPER_SRC "${VCS_SC_WRAPPER_DIR}/sysc/${V_SOURCE_WO_EXT}/${V_SOURCE_WO_EXT}.cpp")
    set(VCS_SC_WRAPPER_INC "${VCS_SC_WRAPPER_DIR}/sysc/include")

    set(CMAKE_VERBOSE_MAKEFILE ON)

    set(VCS_LD_LIBRARY_PATH "${VG_GNU_PACKAGE}/gcc-9.2.0_64-shared/lib64:$ENV{VG_GNU_PACKAGE}/gcc-9.2.0_64-shared/lib:$ENV{LD_LIBRARY_PATH}")
    set(VCS_PATH_MOD "${VG_GNU_PACKAGE}/gcc-9.2.0_64-shared/bin/:$ENV{VG_GNU_PACKAGE}/binutils-2.33.1_64/bin/:$ENV{PATH}}")

    add_custom_target(vlogan_copy_default_portmap
                        COMMAND ${CMAKE_COMMAND} -E copy
                        $ENV{SC_UVM_ENV_HOME}/cmake/synopsys/cosim_defaults.map
                        ${CMAKE_CURRENT_BINARY_DIR}/cosim_defaults.map)

    add_custom_command(OUTPUT ${VCS_SC_WRAPPER_SRC}
                       COMMAND ${CMAKE_COMMAND} -E env "LD_LIBRARY_PATH=${VCS_LD_LIBRARY_PATH} PATH=${VCS_PATH_MOD}" 
                       vlogan -full64 -nc -sverilog
                       -cpp $ENV{VCS_CXX}
                       -sc_model ${V_SOURCE_WO_EXT} ${TOP_V_FILE}
                       DEPENDS ${V_SOURCES}
                       COMMENT "Generating VCS SystemC wrapper for Verilog module"
                       )

    add_custom_target(vlog 
                      DEPENDS ${VCS_SC_WRAPPER_SRC}
                     )
    set_property(TARGET vlog
        APPEND PROPERTY ADDITIONAL_CLEAN_FILES ${PROJECT_BINARY_DIR}/csrc ${PROJECT_BINARY_DIR}/AN.DB
        )
    add_dependencies(vlog vlogan_copy_default_portmap)

    add_custom_target(vcscan  VERBATIM
                      COMMAND ${CMAKE_COMMAND} -E env "LD_LIBRARY_PATH=${VCS_LD_LIBRARY_PATH} PATH=${VCS_PATH_MOD}"
                      syscan -full64
                      -cpp $ENV{VCS_CXX} -cc $ENV{VCS_CC}
                      -sysc=scv20
                      ${ENV_SOURCES}
                      -CFLAGS "${CFLAGS_INC_DIRS} -O3"
                      DEPENDS vlog
                      )

    list(POP_FRONT V_SOURCES V_SOURCES_NO_TOP)
    add_custom_target(vcsrun  VERBATIM
                      COMMAND ${CMAKE_COMMAND} -E env "LD_LIBRARY_PATH=${VCS_LD_LIBRARY_PATH} PATH=${VCS_PATH_MOD}"
                      vcs -full64 -nc -j16
                      -cpp $ENV{VCS_CXX} -cc $ENV{VCS_CC}
                      -sysc=scv20 sc_main
                      -LDFLAGS "${VCS_LDFLAGS}" ${V_SOURCES}
                      +vcs+dumpvars+wave.vcd
                      -o vcs_tb
                      DEPENDS vcscan)
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
endfunction(vcs)
