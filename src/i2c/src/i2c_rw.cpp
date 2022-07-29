#include <systemc>
#include <uvm>

#include "i2c_rw.h"

void i2c_rw::do_print(const uvm::uvm_printer& printer) const
{
    printer.print_field_int("addr", addr.to_uint());
    printer.print_field_int("data", data.to_uint());
    // printer.print_field_int("strb", strb.to_uint());
    printer.print_string("read_write", kind_e == i2c::READ ? "READ" : "WRITE");
    printer.print_field_int("valid", valid);
    printer.print_field_int("stop condition", stop_cond);
}

void i2c_rw::do_pack(uvm::uvm_packer& p) const
{
    p << addr << data  << (uint32_t)kind_e << valid << stop_cond;
}

void i2c_rw::do_unpack(uvm::uvm_packer& p)
{
    int rw;
    p >> addr >> data >> valid  >> stop_cond >> rw;
    kind_e = (i2c::i2c_rw_enum)rw;
}

void i2c_rw::do_copy(const uvm::uvm_object& rhs)
{
    const i2c_rw* drhs = dynamic_cast<const i2c_rw*>(&rhs);
    if (!drhs) {
        UVM_FATAL("DO_COPY", "Object not of type i2c_rw");
    }

    addr = drhs->addr;
    // copy(rhs.begin(), rhs.end(), back_inserter(data));
    kind_e = drhs->kind_e;
    valid = drhs->valid;
    stop_cond = drhs->stop_cond;
}

bool i2c_rw::do_compare(const uvm_object& rhs) const
{
    const i2c_rw* drhs = dynamic_cast<const i2c_rw*>(&rhs);
    if (!drhs) {
        UVM_FATAL("DO_COPY", "Object not of type i2c_rw");
    }

    return (addr == drhs->addr && data == drhs->data &&  kind_e == drhs->kind_e && valid == drhs->valid && stop_cond == drhs->stop_cond);
}

std::string i2c_rw::convert2string() const
{
    std::ostringstream str;
    str << " addr: " << addr.to_uint();
    str << " data: " << data.to_uint();
    str << " read_write: " << (kind_e == i2c::READ ? "READ" : "WRITE");
    str << " valid: " << valid;
    str << " stop_cond: " << stop_cond;
    return str.str();
}
