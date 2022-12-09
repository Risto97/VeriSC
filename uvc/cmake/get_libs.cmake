
find_file(libs_conf_cmake "libs_conf.cmake"
    PATHS "${LIBS_CFG_DIR}" NO_DEFAULT_PATH)
include(${libs_conf_cmake})

find_file(crave_conf_cmake "crave_conf.cmake"
    PATHS "${LIBS_CFG_DIR}/crave" NO_DEFAULT_PATH
    NO_CACHE
    )
include(${crave_conf_cmake})

MESSAGE(STATUS "LIBS_CONFG : ${LIBS_CFG_DIR}")
MESSAGE(STATUS "LIBS_CONFG sc : ${SYSTEMC_HOME_${INSTALL_GROUP}}")
MESSAGE(STATUS "LIBS_CONFG sc : ${SYSTEMC_HOME_${INSTALL_GROUP}}/lib-linux64/")

MESSAGE(STATUS "LIBS_CONFG  uvm: ${SYSTEMC_UVM_HOME_${INSTALL_GROUP}}")
MESSAGE(STATUS "LIBS_CONFG scv: ${SCV_HOME_${INSTALL_GROUP}}")

find_library(systemc_lib systemc PATHS "${SYSTEMC_HOME_${INSTALL_GROUP}}/lib-linux64/" NO_DEFAULT_PATH)

find_library(sc_uvm_lib uvm-systemc PATHS "/scratch/rpejasin/synopsys_systemc/uvm-systemc-1.0-beta4/lib-linux64/" NO_DEFAULT_PATH)
# find_library(sc_uvm_lib uvm-systemc PATHS "${SYSTEMC_UVM_HOME_${INSTALL_GROUP}}/lib-linux64/" NO_DEFAULT_PATH)

# find_library(scv_lib scv PATHS "${SCV_HOME_${INSTALL_GROUP}}/lib-linux64/" NO_DEFAULT_PATH)

# list(APPEND LIBS ${systemc_lib} ${sc_uvm_lib} ${scv_lib})
list(APPEND LIBS ${systemc_lib} ${sc_uvm_lib})
list(APPEND LIB_INCLUDES "${SYSTEMC_HOME_${INSTALL_GROUP}}/include")
list(APPEND LIB_INCLUDES "${SYSTEMC_UVM_HOME_${INSTALL_GROUP}}/include")
list(APPEND LIB_INCLUDES "${SCV_HOME_${INSTALL_GROUP}}/include")
list(APPEND LIB_INCLUDES "${CRAVE_HOME_${INSTALL_GROUP}}/include")
