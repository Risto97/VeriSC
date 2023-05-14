# This file is loaded at the start of the configuration of VeriSC
# It defines default versions for dependencies
# To override these versions and paths invoke cmake with -DVERISC_CFG=<path-to-config-file>
# VERISC_CFG file will then be included after this one, and variables will be overwritten
# You can set VCS_HOME in the VERISC_CFG file also, or provide it with cmake -DVCS_HOME=

if(NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 17)
endif()

# Select version of dependencies you want to be built
# If in addition to version <DEP>_HOME is provided pointing to the root directory of the library or tool,
# then dependency will not be compiled, instead the one provided below will be used, 
if(NOT SYSTEMC_VERSION)
    set(SYSTEMC_VERSION 2.3.3)
endif()
# set(SYSTEMC_HOME <path-to-systemc>)
if(NOT SCV_VERSION)
    set(SCV_VERSION 2.0.1)
endif()
if(NOT UVM-SYSTEMC_VERSION)
    set(UVM-SYSTEMC_VERSION 1.0-beta4)
endif()
if(NOT VERILATOR_VERSION)
    set(VERILATOR_VERSION v5.006)
endif()
if(NOT FC4SC_VERSION)
    set(FC4SC_VERSION master)
endif()
if(NOT ICSC_COMPILER_VERSION)
    set(ICSC_COMPILER_VERSION master)
endif()

# if you want to disable a dependency from being used set variable set(<DEP>_VERSION FALSE)
# set(ICSC_COMPILER_VERSION FALSE)
# set(UVM-SYSTEMC_VERSION FALSE)
# set(SCV_VERSION FALSE)
# set(FC4SC_VERSION FALSE)

# VeriSC will try to use GCC from the system if it satisfies the CMAKE_CXX_STANDARD
# If not it will compile the following version, otherwise GCC_VERSION is ignored
if(NOT GCC_VERSION)
    set(GCC_VERSION 12.2.0)
endif()
# Example to use preinstalled GCC
# set(GCC_HOME /mnt/ext/verisc/gcc-12.2.0/)
