#include "sysc/kernel/sc_simcontext.h"
#include "sysc/kernel/sc_status.h"
#include "sysc/tracing/sc_trace.h"
#include "verilated_vcd_sc.h"
#include <systemc>
#include <uvm>

#include "Vnmi2apb.h"
#include "env.h"

void env::build_phase(uvm::uvm_phase &phase) {

  uvm::uvm_env::build_phase(phase);
  uvm::uvm_config_db_options::turn_on_tracing();

  apb_vif = new apb_if("apb_slave", &clk);
  nmi_vif = new nmi_if("nmi_master", &clk);

  apb_ag = new apb_agent("apb_agent_slave");
  uvm::uvm_config_db<int>::set(this, "apb_agent_slave", "is_active",
                               uvm::UVM_ACTIVE);
  uvm::uvm_config_db<apb::mode_t>::set(this, "apb_agent_slave", "mode",
                                       apb::SLAVE);
  uvm::uvm_config_db<apb_if *>::set(this, "apb_agent_slave", "vif", apb_vif);
  nmi_ag = new nmi_agent("nmi_agent_master");
  uvm::uvm_config_db<int>::set(this, "nmi_agent_master", "is_active",
                               uvm::UVM_ACTIVE);
  uvm::uvm_config_db<nmi::mode_t>::set(this, "nmi_agent_master", "mode",
                                       nmi::MASTER);
  uvm::uvm_config_db<nmi_if *>::set(this, "nmi_agent_master", "vif", nmi_vif);

  // apb_seq = apb_sequence::type_id::create("apb_seq");
  nmi_seq = nmi_sequence::type_id::create("nmi_seq");

  int verbosity;
  if (!uvm::uvm_config_db<int>::get(this, "*", "verbosity_level", verbosity))
    UVM_FATAL(this->name(), "Verbosity_level not defined! Simulation aborted!");

  sb = scoreboard::type_id::create("sb", this);
  assert(sb);

  apb_mem_i = new apb_mem_slave("apb_mem_i");

  set_report_verbosity_level_hier(verbosity);
  set_report_verbosity_level(verbosity);
}

void env::connect_phase(uvm::uvm_phase &phase) {
  dut.clk_i(clk);
  dut.rst_ni(rstn);

  dut.mem_valid_i(nmi_vif->mem_valid);
  dut.mem_ready_o(nmi_vif->mem_ready);
  dut.mem_addr_i(nmi_vif->mem_addr);
  dut.mem_wdata_i(nmi_vif->mem_wdata);
  dut.mem_wstrb_i(nmi_vif->mem_wstrb);
  dut.mem_rdata_o(nmi_vif->mem_rdata);

  dut.psel_o(apb_vif->psel);
  dut.penable_o(apb_vif->penable);
  dut.pwrite_o(apb_vif->pwrite);
  dut.pready_i(apb_vif->pready);
  dut.paddr_o(apb_vif->paddr);
  dut.pwdata_o(apb_vif->pwdata);
  dut.pstrb_o(apb_vif->pstrb);
  dut.prdata_i(apb_vif->prdata);

  apb_mem_i->pclk(clk);
  apb_mem_i->psel(apb_vif->psel);
  apb_mem_i->penable(apb_vif->penable);
  apb_mem_i->pwrite(apb_vif->pwrite);
  apb_mem_i->pready(apb_vif->pready);
  apb_mem_i->paddr(apb_vif->paddr);
  apb_mem_i->pwdata(apb_vif->pwdata);
  apb_mem_i->pstrb(apb_vif->pstrb);
  apb_mem_i->prdata(apb_vif->prdata);

  apb_ag->mon->ap.connect(sb->apb_listener_imp);
  nmi_ag->mon->ap.connect(sb->nmi_listener_imp);
}

void env::reset_phase(uvm::uvm_phase &phase) {
  phase.raise_objection(this);
  reset();
  phase.drop_objection(this);
}

void env::start_apb_seq() {
  apb_seq->start(apb_ag->sqr);
  apb_seq->wait_for_sequence_state(uvm::UVM_FINISHED);
}
void env::start_nmi_seq() {
  nmi_seq->start(nmi_ag->sqr);
  nmi_seq->wait_for_sequence_state(uvm::UVM_FINISHED);
}

void env::final_phase(uvm::uvm_phase &phase) { 
    close_trace(); 
}

void env::main_phase(uvm::uvm_phase &phase) {
  phase.raise_objection(this);

  VerilatedVcdSc* tfp = nullptr;
  tfp = new VerilatedVcdSc;
  dut.trace(tfp, 99);
  tfp->open("trace.vcd");

  // sc_core::sc_start();
  SC_FORK
  // sc_core::sc_spawn(sc_core::sc_bind(&env::start_apb_seq, this)),
      sc_core::sc_spawn(sc_core::sc_bind(&env::start_nmi_seq, this)),
      // sc_core::sc_spawn(sc_core::sc_bind(&env::timeout, this)),
  SC_JOIN

          sc_core::wait(100 * T);

  tfp->flush();
  tfp->close();
    tfp = nullptr;
  phase.drop_objection(this);
}

void env::timeout() {
  sc_core::wait(300 * T);
  // close_trace();
  sc_core::sc_stop();
}

void env::reset(void) {
  rstn = 0;
  sc_core::wait(2 * T);
  rstn = 1;
  // sc_core::wait(1.5 * T);
}

void env::open_trace(const char *vcdname) {
  // dut.trace(m_trace, 10);
  // m_trace->open(vcdname);
}

void env::close_trace(void) {
  // m_trace->close();
  // m_trace = NULL;
}
