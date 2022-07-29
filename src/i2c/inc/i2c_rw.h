#ifndef I2C_RW_H_
#define I2C_RW_H_

#include <systemc>
#include <uvm>
#include <vector>

namespace i2c{
    typedef enum {
        READ = 1,
        WRITE = 0
    } i2c_rw_enum;
}

class i2c_rw : public uvm::uvm_sequence_item {
    public:
        sc_dt::sc_lv<8> addr;
        sc_dt::sc_lv<8> data;
        i2c::i2c_rw_enum kind_e;
        bool valid;
        bool stop_cond;
        bool error;

        UVM_OBJECT_UTILS(i2c_rw);

        i2c_rw(const std::string & name = "i2c_rw") :
            uvm::uvm_sequence_item(name) {
                valid = false;
                stop_cond =false;
                addr = 0;
                data = 0;
                error = false;
            }

        virtual void do_print(const uvm::uvm_printer& printer) const;
        virtual void do_pack(uvm::uvm_packer& p) const;
        virtual void do_unpack(uvm::uvm_packer& p);
        virtual void do_copy(const uvm::uvm_object& rhs);
        virtual bool do_compare(const uvm_object& rhs) const;
        std::string convert2string() const;
};

#endif 
