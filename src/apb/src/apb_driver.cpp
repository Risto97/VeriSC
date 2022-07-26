#include <stdint.h>
#include <string>
#include <systemc>
#include <uvm>

#include "apb_agent.h"
#include "apb_driver.h"
#include "apb_if.h"
#include "apb_modes.h"
#include "apb_rw.h"

apb_driver::apb_driver(uvm::uvm_component_name name)
    : uvm::uvm_driver<apb_rw>(name), sigs(nullptr) {}

void apb_driver::build_phase(uvm::uvm_phase &phase) {
  UVM_INFO("APB DRIVER build PHASE", "", uvm::UVM_MEDIUM);

  uvm::uvm_driver<apb_rw>::build_phase(phase);

  apb_agent *agent = dynamic_cast<apb_agent *>(get_parent());
  if (agent) {
    sigs = agent->vif;
    mode = agent->mode;
    max_retry_count = agent->max_retry_count;
  } else {
    if (!uvm::uvm_config_db<Apb_if *>::get(this, "", "vif", sigs)) {
      UVM_FATAL("APB/DRV/NOVIF",
                "No virtual interface specified for this driver instance");
    }
    if (!uvm::uvm_config_db<apb::mode_t>::get(this, "", "mode", mode)) {
      UVM_FATAL("APB", "No mode defined, aborting simulation");
    }
  }
}

void apb_driver::run_phase(uvm::uvm_phase &phase) {

  if (mode == apb::MASTER) {
    UVM_INFO("CONFIGURED AS MASTER", "", uvm::UVM_MEDIUM);
    sigs->psel = 0;
    sigs->penable = 0;

    while (1) {
      apb_rw req;

      seq_item_port->get_next_item(req);

      switch (req.kind_e) {
      case READ:
        read(req.addr, req.data, req.strb, req.valid);
        break;
      case WRITE:
        write(req.addr, req.data, req.strb, req.valid);
        break;
      }

      seq_item_port->item_done();
      trig.notify();
    }
  } else if (mode == apb::SLAVE) {
    UVM_INFO("CONFIGURED AS SLAVE", "", uvm::UVM_MEDIUM);
    sigs->pready = 1;
    while (1) {
      sc_core::wait(sigs->pclk->posedge_event());
    }
  }
}

void apb_driver::read(const sc_dt::sc_lv<32> &addr, sc_dt::sc_lv<32> &data,
                      const sc_dt::sc_lv<4> &strb, const bool valid) {
  sigs->paddr = addr;
  sigs->pwrite = 0;
  sigs->pstrb = strb;
  sigs->psel = valid;
  sc_core::wait(sigs->pclk->posedge_event());
  sigs->penable = valid;
  sc_core::wait(sigs->pclk->posedge_event());
  data = sigs->prdata;
  sigs->psel = 0;
  sigs->penable = 0;
}

void apb_driver::write(const sc_dt::sc_lv<32> &addr,
                       const sc_dt::sc_lv<32> &data,
                       const sc_dt::sc_lv<4> &strb, const bool valid) {
  uint32_t retry_count=0;
  sigs->paddr = addr;
  sigs->pwdata = data;
  sigs->pstrb = strb;
  sigs->pwrite = 1;
  sigs->psel = valid;
  sc_core::wait(sigs->pclk->posedge_event());
  sigs->penable = valid;
  do{
      sc_core::wait(sigs->pclk->posedge_event());
      retry_count++;
  }
  while(!sigs->pready && retry_count != max_retry_count);
  if(retry_count == max_retry_count)
      UVM_ERROR("SLAVE STALLED", "SLAVE HAS STALLED");
  
  sigs->psel = 0;
  sigs->penable = 0;
}
