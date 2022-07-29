#ifndef I2C_SEQUENCER_H_
#define I2C_SEQUENCER_H_


#include <systemc>
#include <uvm>

#include "i2c_rw.h"

class i2c_sequencer : public uvm::uvm_sequencer<i2c_rw>
{
    public:
        UVM_COMPONENT_UTILS(i2c_sequencer);

        i2c_sequencer(uvm::uvm_component_name name) :
            uvm::uvm_sequencer<i2c_rw>(name) {}
};


#endif /* I2C_SEQUENCER_H_ */
