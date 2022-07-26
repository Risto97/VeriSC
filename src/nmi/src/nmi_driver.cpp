#include <systemc>
#include <uvm>

#include "nmi_agent.h"
#include "nmi_driver.h"
#include "nmi_if.h"
#include "nmi_rw.h"
#include "sysc/datatypes/int/sc_nbdefs.h"
#include "sysc/kernel/sc_time.h"

nmi_driver::nmi_driver(uvm::uvm_component_name name)
    : uvm::uvm_driver<nmi_rw>(name), sigs(nullptr) {}

void nmi_driver::build_phase(uvm::uvm_phase &phase) {
  uvm::uvm_driver<nmi_rw>::build_phase(phase);

  nmi_agent *agent = dynamic_cast<nmi_agent *>(get_parent());
  if (agent) {
    sigs = agent->vif;
    mode = agent->mode;
  } else {
    if (!uvm::uvm_config_db<nmi_if *>::get(this, "", "vif", sigs)) {
      UVM_FATAL("NMI/DRV/NOVIF",
                "No virtual interface specified for this driver instance");
    }
    if (!uvm::uvm_config_db<nmi::mode_t>::get(this, "", "mode", mode)) {
      UVM_FATAL("NMI", "No mode defined, aborting simulation");
    }
  }
}

void nmi_driver::run_phase(uvm::uvm_phase &phase) {
  sigs->mem_valid = 0;
  sigs->mem_instr = 0;

  while (1) {
    nmi_rw req;
    nmi_rw rsp;

    seq_item_port->get_next_item(req);

    std::string str = req.convert2string();

    switch (req.kind_e) {
    case nmi::READ:
      rsp = read(req);
      break;
    case nmi::WRITE:
      write(req);
      break;
    case nmi::INSN:
      rsp = read_insn(req);
      break;
    }

    if (req.kind_e == nmi::nmi_rw_enum::READ) {
      rsp.set_id_info(req);
      seq_item_port->put_response(rsp);
    }
    seq_item_port->item_done();

    sigs->mem_valid = 0;
    sigs->mem_instr = 0;
  }
}

nmi_rw nmi_driver::read(const nmi_rw req) // TODO retry num
{
  nmi_rw rsp;
  sigs->mem_addr = req.addr;
  sigs->mem_wstrb = 0;
  sigs->mem_instr = 0;
  sigs->mem_valid = req.valid;

  if (req.valid)
    while (sigs->mem_ready == 0)
      sc_core::wait(sigs->clk->posedge_event());
  else
    sc_core::wait(sigs->clk->posedge_event());

  rsp.valid = true;
  rsp.addr = req.addr;
  rsp.data = sigs->mem_rdata;
  rsp.kind_e = req.kind_e;

  return rsp;
}

nmi_rw nmi_driver::read_insn(const nmi_rw req) // TODO retry num
{
  nmi_rw rsp;
  sigs->mem_addr = req.addr;
  sigs->mem_wstrb = 0;
  sigs->mem_instr = 1;
  sigs->mem_valid = req.valid;

  while (sigs->mem_ready == 0)
    sc_core::wait(sigs->clk->posedge_event());

  rsp.data = sigs->mem_rdata;
  sc_core::wait(sigs->clk->posedge_event());

  rsp.valid = true;
  rsp.kind_e = req.kind_e;
  rsp.addr = req.addr;
  rsp.data = sigs->mem_rdata;

  return rsp;
}

void nmi_driver::write(const nmi_rw req) // TODO retry num
{
  sigs->mem_addr = req.addr;
  sigs->mem_wdata = req.data;
  sigs->mem_wstrb = req.strb;
  sigs->mem_instr = 0;
  sigs->mem_valid = req.valid;

  if (req.valid)
    while (sigs->mem_ready == 0)
      sc_core::wait(sigs->clk->posedge_event());
  else
    sc_core::wait(sigs->clk->posedge_event());

  // sc_core::wait(sigs->clk->posedge_event());
}
