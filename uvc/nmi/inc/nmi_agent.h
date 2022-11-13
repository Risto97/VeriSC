#ifndef NMI_AGENT_H_
#define NMI_AGENT_H_

#include <systemc>
#include <uvm>

#include "nmi_sequencer.h"
#include "nmi_driver.h"
#include "nmi_monitor.h"
#include "nmi_modes.h"


class nmi_agent : public uvm::uvm_agent
{
    public:
        nmi_sequencer* sqr;
        nmi_driver* drv;
        nmi_monitor* mon;

        nmi::mode_t mode;

        nmi_if* vif;

        UVM_COMPONENT_UTILS(nmi_agent);

        nmi_agent(uvm::uvm_component_name name);
        ~nmi_agent();

        virtual void build_phase(uvm::uvm_phase& phase);

        virtual void connect_phase(uvm::uvm_phase& phase);

};

#endif /* NMI_AGENT_H */
