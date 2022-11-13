
set(CMAKE_FIND_DEBUG_MODE True)
set(CMAKE_FIND_DEBUG_MODE False)
find_file(file "libs_conf_${CMAKE_CXX_COMPILER_VERSION}.cmake"
            PATHS "$ENV{SC_UVM_ENV_HOME}/*/*" NO_DEFAULT_PATH)

include(${file})

set(TAG ${CMAKE_CXX_COMPILER_VERSION})

find_library(systemc_lib systemc PATHS "${SYSTEMC_HOME_${TAG}}/*" NO_DEFAULT_PATH)

find_library(sc_uvm_lib uvm-systemc PATHS "${SYSTEMC_UVM_HOME_${TAG}}/*" NO_DEFAULT_PATH)

find_library(scv_lib scv PATHS "${SCV_HOME_${TAG}}/*" NO_DEFAULT_PATH)

list(APPEND LIBS ${systemc_lib} ${sc_uvm_lib} ${scv_lib})
list(APPEND LIB_INCLUDES "${SYSTEMC_HOME_${TAG}}/include")
list(APPEND LIB_INCLUDES "${SYSTEMC_UVM_HOME_${TAG}}/include")
list(APPEND LIB_INCLUDES "${SCV_HOME_${TAG}}/include")
