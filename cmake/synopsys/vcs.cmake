
# Synopsys VCS
message(STATUS "CUrrent binary dir ${CMAKE_CURRENT_BINARY_DIR}")
add_custom_target(vlogan_copy_default_portmap
                    COMMAND ${CMAKE_COMMAND} -E copy
                    $ENV{SC_UVM_ENV_HOME}/cmake/synopsys/cosim_defaults.map
                    ${CMAKE_CURRENT_BINARY_DIR}/cosim_defaults.map)
