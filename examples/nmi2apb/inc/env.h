#ifndef ENV_HPP_
#define ENV_HPP_

#include "verilated_vcd_sc.h"
#include <systemc>
#include <uvm>

#include "Vnmi2apb.h"

#include "scoreboard.h"
#include <apb.h>
#include <apb_agent.h>
#include <apb_if.h>
#include <apb_modes.h>
#include <apb_rw.h>
#include <apb_sequence.h>
#include <nmi.h>
#include <nmi_agent.h>
#include <nmi_if.h>
#include <nmi_modes.h>
#include <nmi_sequence.h>
#include "apb_mem_slave.h"

// template <class MODULE>
class env : public uvm::uvm_env {
public:
  Vnmi2apb dut;
  // VerilatedVcdSc *m_trace;

  sc_core::sc_time T;

  sc_core::sc_clock clk;
  sc_core::sc_signal<bool> rstn;

  sc_signal<bool> bcd;

  apb_if *apb_vif;
  nmi_if *nmi_vif;

  apb_agent *apb_ag;
  nmi_agent *nmi_ag;

  apb_sequence *apb_seq;
  nmi_sequence *nmi_seq;

  scoreboard *sb;

  apb_mem_slave *apb_mem_i;

  UVM_COMPONENT_UTILS(env);

  env(uvm::uvm_component_name name)
      : uvm::uvm_env(name), dut("nmi2apb"), T(10, SC_NS), clk("clk", T),
        rstn("rstn") {
    // m_trace = new VerilatedVcdSc;
    Verilated::traceEverOn(true);
  }

  void build_phase(uvm::uvm_phase &phase);

  void connect_phase(uvm::uvm_phase &phase);

  void reset_phase(uvm::uvm_phase &phase);

  void start_apb_seq();

  void start_nmi_seq();

  void timeout();

  void final_phase(uvm::uvm_phase &phase);

  virtual void main_phase(uvm::uvm_phase &phase);

  virtual void reset(void);

  virtual void open_trace(const char *vcdname);
  virtual void close_trace(void);
};

#endif
