#ifndef NMI_SEQUENCER_H_
#define NMI_SEQUENCER_H_

#include <systemc>
#include <uvm>

#include "nmi_rw.h"

class nmi_sequencer : public uvm::uvm_sequencer<nmi_rw>
{
    public:
        UVM_COMPONENT_UTILS(nmi_sequencer);

        nmi_sequencer(uvm::uvm_component_name name) :
            uvm::uvm_sequencer<nmi_rw>(name) {}
};


#endif /* NMI_SEQUENCER_H_ */
