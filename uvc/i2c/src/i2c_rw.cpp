#include <systemc>
#include <uvm>

#include "i2c_rw.h"

namespace uvc::i2c{
    void rw::do_print(const uvm::uvm_printer& printer) const
    {
        printer.print_field_int("addr", addr.to_uint());
        printer.print_field_int("data", data.to_uint());
        // printer.print_field_int("strb", strb.to_uint());
        printer.print_string("read_write", kind_e == READ ? "READ" : "WRITE");
        printer.print_field_int("valid", valid);
        printer.print_field_int("stop condition", stop_cond);
    }

    void rw::do_pack(uvm::uvm_packer& p) const
    {
        p << addr << data  << (uint32_t)kind_e << valid << stop_cond;
    }

    void rw::do_unpack(uvm::uvm_packer& p)
    {
        int rw;
        p >> addr >> data >> valid  >> stop_cond >> rw;
        kind_e = (rw_enum)rw;
    }

    void rw::do_copy(const uvm::uvm_object& rhs)
    {
        const rw* drhs = dynamic_cast<const rw*>(&rhs);
        if (!drhs) {
            UVM_FATAL("DO_COPY", "Object not of type rw");
        }

        addr = drhs->addr;
        // copy(rhs.begin(), rhs.end(), back_inserter(data));
        kind_e = drhs->kind_e;
        valid = drhs->valid;
        stop_cond = drhs->stop_cond;
    }

    bool rw::do_compare(const uvm_object& rhs) const
    {
        const rw* drhs = dynamic_cast<const rw*>(&rhs);
        if (!drhs) {
            UVM_FATAL("DO_COPY", "Object not of type rw");
        }

        return (addr == drhs->addr && data == drhs->data &&  kind_e == drhs->kind_e && valid == drhs->valid && stop_cond == drhs->stop_cond);
    }

    std::string rw::convert2string() const
    {
        std::ostringstream str;
        str << " addr: " << addr.to_uint();
        str << " data: " << data.to_uint();
        str << " read_write: " << (kind_e == READ ? "READ" : "WRITE");
        str << " valid: " << valid;
        str << " stop_cond: " << stop_cond;
        return str.str();
    }
}
