#include <systemc>
#include <uvm>

#include "nmi_driver.h"
#include "nmi_rw.h"
#include "nmi_if.h"
#include "nmi_agent.h"
#include "sysc/datatypes/int/sc_nbdefs.h"
#include "sysc/kernel/sc_time.h"

nmi_driver::nmi_driver(uvm::uvm_component_name name) :
    uvm::uvm_driver<nmi_rw>(name),
    sigs(nullptr)
{}

void nmi_driver::build_phase(uvm::uvm_phase & phase)
{
    uvm::uvm_driver<nmi_rw>::build_phase(phase);

    nmi_agent* agent = dynamic_cast<nmi_agent*>(get_parent());
    if (agent) {
        sigs = agent->vif;
        mode = agent->mode;
    }
    else {
        if (!uvm::uvm_config_db<nmi_if*>::get(this, "", "vif", sigs)) {
            UVM_FATAL("NMI/DRV/NOVIF", "No virtual interface specified for this driver instance");
        }
        if(!uvm::uvm_config_db<nmi::mode_t>::get(this, "", "mode", mode)){
            UVM_FATAL("NMI", "No mode defined, aborting simulation");
        }
    }
}

void nmi_driver::run_phase(uvm::uvm_phase & phase)
{
    sigs->mem_valid = 0;
    sigs->mem_instr = 0;

    while (1) {
        nmi_rw tr;

        seq_item_port->get_next_item(tr);

        std::string str = tr.convert2string();

        switch (tr.kind_e) {
            case nmi::READ : read(tr.addr, tr.data, tr.valid); break;
            case nmi::WRITE : write(tr.addr, tr.data, tr.strb, tr.valid); break;
            case nmi::INSN : read_insn(tr.addr, tr.data, tr.valid); break;
        }

        seq_item_port->item_done();
        trig.notify();

        sigs->mem_valid = 0;
        sigs->mem_instr = 0;
    }
}

void nmi_driver::read(const sc_dt::sc_lv<32> & addr, sc_dt::sc_lv<32> & data, const bool valid)
{
    sigs->mem_addr = addr;
    sigs->mem_wstrb = 0;
    sigs->mem_instr = 0;
    sigs->mem_valid = valid;

    if(valid)
        while(sigs->mem_ready == 0)
            sc_core::wait(sigs->clk->posedge_event());
    else
        sc_core::wait(sigs->clk->posedge_event());
    
    data = sigs->mem_rdata;

}

void nmi_driver::read_insn(const sc_dt::sc_lv<32> & addr, sc_dt::sc_lv<32> & data, const bool valid)
{
    sigs->mem_addr = addr;
    sigs->mem_wstrb = 0;
    sigs->mem_instr = 1;
    sigs->mem_valid = valid;

    while(sigs->mem_ready == 0)
        sc_core::wait(sigs->clk->posedge_event());
    
    data = sigs->mem_rdata;
    sc_core::wait(sigs->clk->posedge_event());

}

void nmi_driver::write(const sc_dt::sc_lv<32> & addr, const sc_dt::sc_lv<32> & data, const sc_dt::sc_lv<4> & strb, const bool valid)
{
    sigs->mem_addr = addr;
    sigs->mem_wdata = data;
    sigs->mem_wstrb = strb;
    sigs->mem_instr = 0;
    sigs->mem_valid = valid;

    if(valid)
        while(sigs->mem_ready == 0)
            sc_core::wait(sigs->clk->posedge_event());
    else
        sc_core::wait(sigs->clk->posedge_event());

    // sc_core::wait(sigs->clk->posedge_event());

    

}
