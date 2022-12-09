#ifndef __SEQ_LIB_H__
#define __SEQ_LIB_H__

#include <stdint.h>
#include <systemc>
#include <uvm>
#include <crave2uvm.h>

#include <apb.h>


class base_seq : public uvm_randomized_sequence<uvc::apb::rw>{
    public:

    base_seq(crave::crv_object_name name = "base_seq")
        : uvm_randomized_sequence<uvc::apb::rw>(name)
        {
            set_automatic_phase_objection(true);
        }

    UVM_OBJECT_UTILS(base_seq);

};
class rand_seq  : public base_seq {
    public:

        crave::crv_variable<uint32_t> data;
        uvc::apb::rw *req;

        rand_seq(crave::crv_object_name name = "rand_seq")
            : base_seq(name)
            {
                req = uvc::apb::rw::type_id::create("req");
                
            }
        UVM_OBJECT_UTILS(rand_seq);

        virtual void body()
        {
            // UVM_DO_WITH(req, (req->rnd() == data));
            // req->print();
            req->randomize_with(req->rnd() < 100);
            std::cout << "REQ: " << req->rnd.get().to_uint() << "\n";
        }

        ~rand_seq()
        {
            uvc::apb::rw::type_id::destroy(req);
        }
};

#endif
