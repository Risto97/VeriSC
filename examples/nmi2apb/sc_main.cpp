#include <cstdlib>
#include<iostream>
#include <systemc>
#include <uvm>
#include "fc4sc.hpp"
#include <scv.h>

#include "env.h"



int sc_main(int argc, char **argv) {
#ifdef VERILATOR
  Verilated::commandArgs(argc, argv);
  Verilated::traceEverOn(true);
#endif

  if(argc < 2)
      scv_random::set_global_seed(0);
  else
      scv_random::set_global_seed(atoi(argv[1]));
  

  int verbosity = uvm::UVM_MEDIUM;
  uvm::uvm_config_db<int>::set(NULL, "*", "recording_detail", uvm::UVM_LOW);
  uvm::uvm_config_db<int>::set(NULL, "*", "verbosity_level", verbosity);
  uvm::uvm_set_verbosity_level(verbosity);


  env *my_env = new env("env");

  uvm::uvm_default_printer->knobs.reference = 0;

  uvm::uvm_root::get()->print_topology();

  uvm::run_test();

  fc4sc::global::coverage_save("coverage_results.xml");

  delete my_env;

  return 0;
}
