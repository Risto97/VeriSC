#ifndef APB_SEQUENCER_H_
#define APB_SEQUENCER_H_

#include <systemc>
#include <uvm>

#include "apb_rw.h"

namespace uvc::apb{
    class sequencer : public uvm::uvm_sequencer<rw>
    {
        public:
            UVM_COMPONENT_UTILS(sequencer);

            sequencer(uvm::uvm_component_name name) :
                uvm::uvm_sequencer<rw>(name) {}
    };
}

#endif /* APB_SEQUENCER_H_ */
