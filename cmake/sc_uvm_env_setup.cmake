find_file(gcc_conf_cmake "gcc_conf.cmake"
    PATHS "$ENV{SC_UVM_ENV_HOME}/open" NO_DEFAULT_PATH)
include(${gcc_conf_cmake})

find_file(verilator_cmake "verilator.cmake"
    PATHS "$ENV{SC_UVM_ENV_HOME}/cmake" NO_DEFAULT_PATH
    NO_CACHE
    )
include(${verilator_cmake})

find_file(vcs_cmake "vcs.cmake"
    PATHS "$ENV{SC_UVM_ENV_HOME}/cmake/synopsys" NO_DEFAULT_PATH
    NO_CACHE
    )
include(${vcs_cmake})

function(setup)
    verilator()
    vcs()
endfunction()
