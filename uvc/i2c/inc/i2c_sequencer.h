#ifndef I2C_SEQUENCER_H_
#define I2C_SEQUENCER_H_


#include <systemc>
#include <uvm>

#include "i2c_rw.h"

namespace uvc::i2c{
    class sequencer : public uvm::uvm_sequencer<rw>
    {
        public:
            UVM_COMPONENT_UTILS(sequencer);

            sequencer(uvm::uvm_component_name name) :
                uvm::uvm_sequencer<rw>(name) {}
    };
}

#endif /* I2C_SEQUENCER_H_ */
