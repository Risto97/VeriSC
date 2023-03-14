# This file is loaded at the start of the configuration of VeriSC
# It defines default versions for dependencies
# To override these versions and paths invoke cmake with -DVERISC_CFG=<path-to-config-file>
# VERISC_CFG file will then be included after this one, and variables will be overwritten
# You can set VCS_HOME in the VERISC_CFG file also, or provide it with cmake -DVCS_HOME=

set(CMAKE_CXX_STANDARD 17)

# Select version of dependencies you want to be built
# If in addition to version <DEP>_HOME is provided pointing to the root directory of the library or tool,
# then dependency will not be compiled, instead the one provided below will be used, 
set(SYSTEMC_VERSION 2.3.3)
# set(SYSTEMC_HOME <path-to-systemc>)
set(SCV_VERSION 2.0.1)
set(UVM-SYSTEMC_VERSION 1.0-beta4)
set(VERILATOR_VERSION 5.006)
set(FC4SC_VERSION master)
set(ICSC_COMPILER_VERSION master)

# if you want to disable a dependency from being used set variable set(<DEP>_VERSION FALSE)
# set(ICSC_COMPILER_VERSION FALSE)
# set(UVM-SYSTEMC_VERSION FALSE)
# set(SCV_VERSION FALSE)
# set(FC4SC_VERSION FALSE)

# VeriSC will try to use GCC from the system if it satisfies the CMAKE_CXX_STANDARD
# If not it will compile the following version, otherwise GCC_VERSION is ignored
set(GCC_VERSION 12.2.0)
# Example to use preinstalled GCC
# set(GCC_HOME /mnt/ext/verisc/gcc-12.2.0/)
