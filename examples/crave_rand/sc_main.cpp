#include <cstdlib>
#include <iostream>
#include <systemc>
#include <uvm>

#include "env.h"
#include "crave_test_lib.h"



int sc_main(int argc, char **argv) {

  int verbosity = uvm::UVM_MEDIUM;
  uvm::uvm_config_db<int>::set(NULL, "*", "recording_detail", uvm::UVM_LOW);
  uvm::uvm_config_db<int>::set(NULL, "*", "verbosity_level", verbosity);
  uvm::uvm_set_verbosity_level(verbosity);

  uvm::run_test("random_test");

  return 0;
}
