#ifndef APB_AGENT_H_
#define APB_AGENT_H_

#include <systemc>
#include <uvm>

#include "apb.h"
#include "apb_sequencer.h"
#include "apb_driver.h"
#include "apb_monitor.h"
#include "apb_modes.h"



namespace uvc::apb{
    class agent : public uvm::uvm_agent
    {
        public:
            sequencer* sqr;
            driver* drv;
            monitor* mon;

            apb::mode_t mode;
            uint32_t max_retry_count;

            Intf* vif;

            UVM_COMPONENT_UTILS(agent);

            agent(uvm::uvm_component_name name);
            ~agent();

            virtual void build_phase(uvm::uvm_phase& phase);

            virtual void connect_phase(uvm::uvm_phase& phase);

    };
}

#endif /* APB_AGENT_H */
