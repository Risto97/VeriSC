# set(CMAKE_FIND_DEBUG_MODE True)
find_file(verilator_cmake "verilator.cmake"
    PATHS "$ENV{SC_UVM_ENV_HOME}/*" NO_DEFAULT_PATH)
include(${verilator_cmake})
# set(CMAKE_FIND_DEBUG_MODE False)

function(setup)
    verilator()
endfunction()
