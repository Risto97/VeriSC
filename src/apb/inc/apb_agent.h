#ifndef APB_AGENT_H_
#define APB_AGENT_H_

#include <systemc>
#include <uvm>

#include "apb.h"
#include "apb_sequencer.h"
#include "apb_driver.h"
#include "apb_monitor.h"
#include "apb_modes.h"



class apb_agent : public uvm::uvm_agent
{
    public:
        apb_sequencer* sqr;
        apb_driver* drv;
        apb_monitor* mon;

        apb::mode_t mode;

        Apb_if* vif;

        UVM_COMPONENT_UTILS(apb_agent);

        apb_agent(uvm::uvm_component_name name);
        ~apb_agent();

        virtual void build_phase(uvm::uvm_phase& phase);

        virtual void connect_phase(uvm::uvm_phase& phase);

};

#endif /* APB_AGENT_H */
