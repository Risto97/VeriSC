#include <systemc>
#include <uvm>
#include <string>

#include "i2c_agent.h"
#include "i2c_sequencer.h"
#include "i2c_driver.h"
#include "i2c_monitor.h"

namespace uvc::i2c{
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
        uvm::uvm_agent::build_phase(phase);

        mon = monitor::type_id::create("mon", this);
        assert(mon);

        if (get_is_active() == uvm::UVM_ACTIVE)
        {
            sqr = sequencer::type_id::create("sqr", this);
            assert(sqr);
            drv = driver::type_id::create("drv", this);
            assert(drv);
        }

        if (!uvm::uvm_config_db<intf*>::get(this, "", "vif", vif)) {
            UVM_FATAL(this->name(), "No virtual interface specified for this agent instance");
        }

        if(!uvm::uvm_config_db<mode_t>::get(this, "", "mode", mode)){
            UVM_FATAL(this->name(), "No mode defined, aborting simulation");
        }
        uvm::uvm_config_db<int>::set(this, "drv", "mode", mode);


    }

    void agent::connect_phase(uvm::uvm_phase& phase)
    {
        if (get_is_active() == uvm::UVM_ACTIVE) {
            drv->seq_item_port.connect(sqr->seq_item_export);
        }
    }
}
