#include <systemc>
#include <uvm>

#include "apb_rw.h"

void apb_rw::do_print(const uvm::uvm_printer& printer) const
{
    printer.print_field_int("addr", addr.to_uint());
    printer.print_field_int("data", data.to_uint());
    printer.print_field_int("strb", strb.to_uint());
    printer.print_string("read_write", kind_e == READ ? "READ" : "WRITE");
    printer.print_field_int("valid", valid);
    printer.print_field_int("slv_err", slv_err);
}

void apb_rw::do_pack(uvm::uvm_packer& p) const
{
    p << addr << data << strb << (int)kind_e << valid << slv_err;
}

void apb_rw::do_unpack(uvm::uvm_packer& p)
{
    int rw;
    p >> addr >> data >> strb >> valid >> slv_err >> rw;
    kind_e = (apb_rw_enum)rw;
}

void apb_rw::do_copy(const uvm::uvm_object& rhs)
{
    const apb_rw* drhs = dynamic_cast<const apb_rw*>(&rhs);
    if (!drhs) {
        UVM_FATAL("DO_COPY", "Object not of type apb_rw");
    }

    addr = drhs->addr;
    data = drhs->data;
    strb = drhs->strb;
    kind_e = drhs->kind_e;
    valid = drhs->valid;
    slv_err = drhs->slv_err;
}

bool apb_rw::do_compare(const uvm_object& rhs) const
{
    const apb_rw* drhs = dynamic_cast<const apb_rw*>(&rhs);
    if (!drhs) {
        UVM_FATAL("DO_COPY", "Object not of type apb_rw");
    }

    return (addr == drhs->addr && data == drhs->data && strb == drhs->strb && kind_e == drhs->kind_e && valid == drhs->valid && slv_err == drhs->slv_err);
}

std::string apb_rw::convert2string() const
{
    std::ostringstream str;
    str << " addr: " << addr.to_uint();
    str << " data: " << data.to_uint();
    str << " strb: " << strb;
    str << " read_write: " << (kind_e == READ ? "READ" : "WRITE");
    str << " valid: " << valid;
    str << " slv_err: " << slv_err;
    return str.str();
}
