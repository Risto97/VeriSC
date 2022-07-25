#include <systemc>
#include <uvm>
#include <scv.h>
#include "verilated.h"
#include "Vnmi2apb.h"
#include <cstdlib>

#include "env.h"
#include "fc4sc.hpp"

#include<iostream>

int sc_main(int argc, char **argv) {
  Verilated::commandArgs(argc, argv);
  Verilated::traceEverOn(true);

  if(argc < 2)
      scv_random::set_global_seed(0);
  else
      scv_random::set_global_seed(atoi(argv[1]));
  // VerilatedVcdSc m_trace;
  

  int verbosity = uvm::UVM_MEDIUM;
  uvm::uvm_config_db<int>::set(NULL, "*", "recording_detail", uvm::UVM_LOW);
  uvm::uvm_config_db<int>::set(NULL, "*", "verbosity_level", verbosity);
  uvm::uvm_set_verbosity_level(verbosity);


  env *my_env = new env("env");

  // my_env->dut.trace(&m_trace, 10);
  // m_trace.open("trace.vcd");

  uvm::uvm_default_printer->knobs.reference = 0;

  uvm::uvm_root::get()->print_topology();

  uvm::run_test();

  fc4sc::global::coverage_save("coverage_results.xml");

  // m_trace.close();
  delete my_env;

  return 0;
}
