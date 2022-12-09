#ifndef CRAVE_EXAMPLE_TEST_LIB_H_
#define CRAVE_EXAMPLE_TEST_LIB_H_

#include <systemc>
#include <uvm>
#include "env.h"
#include "seq_lib.h"

class base_test : public uvm::uvm_test
{
    public:
        env * tb {nullptr};

        UVM_COMPONENT_UTILS(base_test);

        base_test(uvm::uvm_component_name name = "base_test") : uvm::uvm_test(name)
        {

        }

    virtual void build_phase(uvm::uvm_phase& phase)
    {
        uvm::uvm_test::build_phase(phase);

        tb = env::type_id::create("tb", this);
        assert(tb);

    }

    void run_phase(uvm::uvm_phase& phase){
        sc_core::sc_time drain_time = sc_core::sc_time(50.0, sc_core::SC_NS);
        phase.get_objection()->set_drain_time(this, drain_time);
    }
};

class random_test : public base_test
{
    public:
        UVM_COMPONENT_UTILS(random_test);

        rand_seq *seq {nullptr};
        random_test(uvm::uvm_component_name name = "random_test") : base_test(name) 
        {}

    virtual void build_phase(uvm::uvm_phase& phase){
        base_test::build_phase(phase);
        // uvm::uvm_config_db<uvm::uvm_object_wrapper*>::set(this,
        //   "apb_seq.sequencer.main_phase",
        //   "default_sequence",
        //   loop_read_modify_write_seq::type_id::get() );
        seq = rand_seq::type_id::create("seq");
        assert(tb);
    }

    
    void main_phase(uvm::uvm_phase &phase){
        seq->start(tb->ag->sqr);

    }

};

#endif

