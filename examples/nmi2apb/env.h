#ifndef ENV_HPP_
#define ENV_HPP_

#include <systemc>
#include <uvm>

#include "nmi2apb.h"

#include "scoreboard.h"
#include <apb.h>
#include <nmi.h>
#include "nmi_sequence.h"
#include "apb/apb_mem_slave.h"

#ifdef VERILATOR
#include "verilated_vcd_sc.h"
#endif
// template <class MODULE>
class env : public uvm::uvm_env {
public:
  nmi2apb dut;

#ifdef VERILATOR
  VerilatedVcdSc *m_trace;
#endif

  sc_core::sc_time T;

  sc_core::sc_clock clk;
  sc_core::sc_signal<bool> rstn;

  sc_signal<bool> bcd;

  uvc::apb::Intf *apb_vif;
  uvc::nmi::intf *nmi_vif;

  uvc::apb::agent *apb_ag;
  uvc::nmi::agent *nmi_ag;

  nmi_sequence *nmi_seq;

  scoreboard *sb;

  uvc::apb::mem_slave *apb_mem_i;

  UVM_COMPONENT_UTILS(env);

  env(uvm::uvm_component_name name)
      : uvm::uvm_env(name), dut("nmi2apb"), T(10, SC_NS), clk("clk", T),
        rstn("rstn") {
  }

  void build_phase(uvm::uvm_phase &phase);
  void connect_phase(uvm::uvm_phase &phase);
  void reset_phase(uvm::uvm_phase &phase);
  void start_nmi_seq();
  void final_phase(uvm::uvm_phase &phase);
  virtual void main_phase(uvm::uvm_phase &phase);
  virtual void reset(void);
  virtual void open_trace(const char *vcdname);
  virtual void close_trace(void);
};

#endif
