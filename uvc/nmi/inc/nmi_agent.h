#ifndef NMI_AGENT_H_
#define NMI_AGENT_H_

#include <systemc>
#include <uvm>

#include "nmi_sequencer.h"
#include "nmi_driver.h"
#include "nmi_monitor.h"
#include "nmi_modes.h"


namespace uvc::nmi{
    class agent : public uvm::uvm_agent
    {
        public:
            sequencer* sqr;
            driver* drv;
            monitor* mon;

            mode_t mode;

            intf *vif;

            UVM_COMPONENT_UTILS(agent);

            agent(uvm::uvm_component_name name);
            ~agent();

            virtual void build_phase(uvm::uvm_phase& phase);

            virtual void connect_phase(uvm::uvm_phase& phase);

    };
}

#endif /* NMI_AGENT_H */
