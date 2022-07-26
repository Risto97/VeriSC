#include <systemc>
#include <uvm>
#include <string>

#include "apb_agent.h"
#include "apb_sequencer.h"
#include "apb_driver.h"
#include "apb_monitor.h"

apb_agent::apb_agent(uvm::uvm_component_name name) :
    uvm::uvm_agent(name),
    sqr(nullptr),
    drv(nullptr),
    mon(nullptr),
    vif(nullptr)
{}

apb_agent::~apb_agent()
{
    if (sqr)
    {
        delete sqr;
    }
    if (drv)
    {
        delete drv;
    }
    if (mon)
    {
        delete mon;
    }
}

void apb_agent::build_phase(uvm::uvm_phase& phase)
{
    UVM_INFO("APB AGENT build PHASE", "", uvm::UVM_MEDIUM);
    uvm::uvm_agent::build_phase(phase);

    mon = apb_monitor::type_id::create("mon", this);
    assert(mon);

    if (get_is_active() == uvm::UVM_ACTIVE)
    {
        sqr = apb_sequencer::type_id::create("sqr", this);
        assert(sqr);
        drv = apb_driver::type_id::create("drv", this);
        assert(drv);
    }

    if (!uvm::uvm_config_db<Apb_if*>::get(this, "", "vif", vif)) {
        UVM_FATAL("APB/AGT/NOVIF", "No virtual interface specified for this agent instance");
    }

    if(!uvm::uvm_config_db<apb::mode_t>::get(this, "", "mode", mode)){
        UVM_FATAL("APB", "No mode defined, aborting simulation");
    }
    uvm::uvm_config_db<int>::set(this, "drv", "mode", mode);

    if(!uvm::uvm_config_db<uint32_t>::get(this, "", "max_retry_count", max_retry_count)){
        UVM_INFO("APB", "No max_retry defined, setting to 1", uvm::UVM_LOW);
        max_retry_count = 1;
    }


}

void apb_agent::connect_phase(uvm::uvm_phase& phase)
{
    if (get_is_active() == uvm::UVM_ACTIVE) {
        drv->seq_item_port.connect(sqr->seq_item_export);
    }
}
