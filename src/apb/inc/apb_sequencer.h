#ifndef APB_SEQUENCER_H_
#define APB_SEQUENCER_H_

#include <systemc>
#include <uvm>

#include "apb_rw.h"

class apb_sequencer : public uvm::uvm_sequencer<apb_rw>
{
    public:
        UVM_COMPONENT_UTILS(apb_sequencer);

        apb_sequencer(uvm::uvm_component_name name) :
            uvm::uvm_sequencer<apb_rw>(name) {}
};


#endif /* APB_SEQUENCER_H_ */
