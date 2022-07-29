#ifndef I2C_AGENT_H_
#define I2C_AGENT_H_

#include <systemc>
#include <uvm>

#include "i2c.h"
#include "i2c_sequencer.h"
#include "i2c_driver.h"
#include "i2c_monitor.h"
#include "i2c_modes.h"



class i2c_agent : public uvm::uvm_agent
{
    public:
        i2c_sequencer* sqr;
        i2c_driver* drv;
        i2c_monitor* mon;

        i2c::mode_t mode;

        i2c_if* vif;

        UVM_COMPONENT_UTILS(i2c_agent);

        i2c_agent(uvm::uvm_component_name name);
        ~i2c_agent();

        virtual void build_phase(uvm::uvm_phase& phase);

        virtual void connect_phase(uvm::uvm_phase& phase);

};

#endif
