#ifndef NMI_RW_H_
#define NMI_RW_H_

#include <systemc>
#include <uvm>

namespace uvc::nmi{
    typedef enum {
        READ,
        WRITE,
        INSN
    } rw_enum;

class rw : public uvm::uvm_sequence_item {
    public:
        sc_dt::sc_bv<32> addr;
        sc_dt::sc_bv<32> data;
        sc_dt::sc_bv<4> strb;
        rw_enum kind_e;
        bool valid;

        UVM_OBJECT_UTILS(rw);

        rw(const std::string & name = "nmi_rw") :
            uvm::uvm_sequence_item(name) {}

        virtual void do_print(const uvm::uvm_printer& printer) const;
        virtual void do_pack(uvm::uvm_packer& p) const;
        virtual void do_unpack(uvm::uvm_packer& p);
        virtual void do_copy(const uvm::uvm_object& rhs);
        virtual bool do_compare(const uvm_object& rhs) const;
        std::string convert2string() const;
};
}

#endif /* NMI_RW_ */
