
find_file(libs_conf_cmake "libs_conf.cmake"
    PATHS "${LIBS_CFG_DIR}" NO_DEFAULT_PATH)
include(${libs_conf_cmake})


find_library(systemc_lib systemc PATHS "${SYSTEMC_HOME_${INSTALL_GROUP}}/*" NO_DEFAULT_PATH)

find_library(sc_uvm_lib uvm-systemc PATHS "${SYSTEMC_UVM_HOME_${INSTALL_GROUP}}/*" NO_DEFAULT_PATH)

find_library(scv_lib scv PATHS "${SCV_HOME_${INSTALL_GROUP}}/*" NO_DEFAULT_PATH)

list(APPEND LIBS ${systemc_lib} ${sc_uvm_lib} ${scv_lib})
list(APPEND LIB_INCLUDES "${SYSTEMC_HOME_${INSTALL_GROUP}}/include")
list(APPEND LIB_INCLUDES "${SYSTEMC_UVM_HOME_${INSTALL_GROUP}}/include")
list(APPEND LIB_INCLUDES "${SCV_HOME_${INSTALL_GROUP}}/include")
