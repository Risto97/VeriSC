#include <systemc>
#include <uvm>
#include <string>

#include "apb_agent.h"
#include "apb_sequencer.h"
#include "apb_driver.h"
#include "apb_monitor.h"

namespace uvc::apb{
    agent::agent(uvm::uvm_component_name name) :
        uvm::uvm_agent(name),
        sqr(nullptr),
        drv(nullptr),
        mon(nullptr),
        vif(nullptr)
    {}

    agent::~agent()
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

    void agent::build_phase(uvm::uvm_phase& phase)
    {
        UVM_INFO("APB AGENT build PHASE", "", uvm::UVM_MEDIUM);
        uvm::uvm_agent::build_phase(phase);

        mon = monitor::type_id::create("apb_mon", this);
        assert(mon);

        if (get_is_active() == uvm::UVM_ACTIVE)
        {
            sqr = sequencer::type_id::create("apb_sqr", this);
            assert(sqr);
            drv = driver::type_id::create("apb_drv", this);
            assert(drv);
        }

        if (!uvm::uvm_config_db<Intf*>::get(this, "", "vif", vif)) {
            UVM_FATAL("APB/AGT/NOVIF", "No virtual interface specified for this agent instance");
        }

        if(!uvm::uvm_config_db<mode_t>::get(this, "", "mode", mode)){
            UVM_FATAL("APB", "No mode defined, aborting simulation");
        }
        uvm::uvm_config_db<int>::set(this, "drv", "mode", mode);

        if(!uvm::uvm_config_db<uint32_t>::get(this, "", "max_retry_count", max_retry_count)){
            UVM_INFO("APB", "No max_retry defined, setting to 1", uvm::UVM_LOW);
            max_retry_count = 1;
        }


    }

    void agent::connect_phase(uvm::uvm_phase& phase)
    {
        if (get_is_active() == uvm::UVM_ACTIVE) {
            drv->seq_item_port.connect(sqr->seq_item_export);
        }
    }
}
