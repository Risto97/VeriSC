#include <systemc>
#include <uvm>

#include "nmi_agent.h"
#include "nmi_sequencer.h"
#include "nmi_driver.h"
#include "nmi_monitor.h"

nmi_agent::nmi_agent(uvm::uvm_component_name name) :
    uvm::uvm_agent(name),
    sqr(nullptr),
    drv(nullptr),
    mon(nullptr),
    vif(nullptr)
{}

nmi_agent::~nmi_agent()
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

void nmi_agent::build_phase(uvm::uvm_phase& phase)
{
    uvm::uvm_agent::build_phase(phase);

    mon = nmi_monitor::type_id::create("mon", this);
    assert(mon);

    if (get_is_active() == uvm::UVM_ACTIVE)
    {
        sqr = nmi_sequencer::type_id::create("sqr", this);
        assert(sqr);
        drv = nmi_driver::type_id::create("drv", this);
        assert(drv);
    }

    if (!uvm::uvm_config_db<nmi_if*>::get(this, "", "vif", vif)) {
        UVM_FATAL("NMI/AGT/NOVIF", "No virtual interface specified for this agent instance");
    }

    if(!uvm::uvm_config_db<nmi::mode_t>::get(this, "", "mode", mode)){
        UVM_FATAL("NMI", "No mode defined, aborting simulation");
    }

    uvm::uvm_config_db<int>::set(this, "drv", "mode", mode);
}

void nmi_agent::connect_phase(uvm::uvm_phase& phase)
{
    if (get_is_active() == uvm::UVM_ACTIVE) {
        drv->seq_item_port.connect(sqr->seq_item_export);
    }
}
