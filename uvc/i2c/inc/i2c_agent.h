#ifndef I2C_AGENT_H_
#define I2C_AGENT_H_

#include <systemc>
#include <uvm>

#include "i2c_sequencer.h"
#include "i2c_driver.h"
#include "i2c_monitor.h"
#include "i2c_modes.h"

namespace uvc::i2c{
    class agent : public uvm::uvm_agent
    {
        public:
            sequencer* sqr;
            driver* drv;
            monitor* mon;

            mode_t mode;

            intf* vif;

            UVM_COMPONENT_UTILS(agent);

            agent(uvm::uvm_component_name name);
            ~agent();

            virtual void build_phase(uvm::uvm_phase& phase);

            virtual void connect_phase(uvm::uvm_phase& phase);

    };
}

#endif
