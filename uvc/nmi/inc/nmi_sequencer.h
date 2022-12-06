#ifndef NMI_SEQUENCER_H_
#define NMI_SEQUENCER_H_

#include <systemc>
#include <uvm>

#include "nmi_rw.h"

namespace uvc::nmi{
    class sequencer : public uvm::uvm_sequencer<rw>
    {
        public:
            UVM_COMPONENT_UTILS(sequencer);

            sequencer(uvm::uvm_component_name name) :
                uvm::uvm_sequencer<rw>(name) {}
    };
}


#endif /* NMI_SEQUENCER_H_ */
