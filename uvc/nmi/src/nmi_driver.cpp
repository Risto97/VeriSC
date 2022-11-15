#include <systemc>
#include <uvm>

#include "nmi_agent.h"
#include "nmi_driver.h"
#include "nmi_if.h"
#include "nmi_rw.h"
#include "sysc/datatypes/int/sc_nbdefs.h"
#include "sysc/kernel/sc_time.h"

namespace uvc::nmi{
    driver::driver(uvm::uvm_component_name name)
        : uvm::uvm_driver<rw>(name), sigs(nullptr) {}

    void driver::build_phase(uvm::uvm_phase &phase) {
      uvm::uvm_driver<rw>::build_phase(phase);

      UVM_INFO(this->get_name(), "Build phase", uvm::UVM_MEDIUM);
      agent *ag = dynamic_cast<agent *>(get_parent());
      if (ag) {
        sigs = ag->vif;
        mode = ag->mode;
      } else {
        if (!uvm::uvm_config_db<intf *>::get(this, "", "vif", sigs)) {
          UVM_FATAL("NMI/DRV/NOVIF",
                    "No virtual interface specified for this driver instance");
        }
        if (!uvm::uvm_config_db<mode_t>::get(this, "", "mode", mode)) {
          UVM_FATAL("NMI", "No mode defined, aborting simulation");
        }
      }
    }

    void driver::run_phase(uvm::uvm_phase &phase) {
      sigs->mem_valid = 0;
      sigs->mem_instr = 0;

      UVM_INFO(this->get_name(), "Starting run phase", uvm::UVM_MEDIUM);
      while (1) {
        rw req;
        rw rsp;

      UVM_INFO(this->get_name(), "Getting item", uvm::UVM_MEDIUM);
        seq_item_port->get_next_item(req);
      UVM_INFO(this->get_name(), "Got item", uvm::UVM_MEDIUM);

        std::string str = req.convert2string();

        switch (req.kind_e) {
        case READ:
          rsp = read(req);
          break;
        case WRITE:
          write(req);
          break;
        case INSN:
          rsp = read_insn(req);
          break;
        }

        if (req.kind_e == rw_enum::READ) {
          rsp.set_id_info(req);
          seq_item_port->put_response(rsp);
        }
        seq_item_port->item_done();

        sigs->mem_valid = 0;
        sigs->mem_instr = 0;
      }
    }

    rw driver::read(const rw req) // TODO retry num
    {
      rw rsp;
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

    rw driver::read_insn(const rw req) // TODO retry num
    {
      rw rsp;
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

    void driver::write(const rw req) // TODO retry num
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
}
