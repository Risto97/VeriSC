#include <systemc>
#include <uvm>
#include <string>

#include "nmi_rw.h"

void nmi_rw::do_print(const uvm::uvm_printer& printer) const
{
    printer.print_field_int("addr", addr.to_uint());
    printer.print_field_int("data", data.to_uint());
    printer.print_field_int("strb", strb.to_uint());
    printer.print_field_int("valid", valid);
    
    std::string kind_str;
    if(kind_e == nmi::READ)
        kind_str = "READ";
    else if(kind_e == nmi::WRITE)
        kind_str = "WRITE";
    else
        kind_str = "INSN";
        
    printer.print_string("read_write", kind_str);
}

void nmi_rw::do_pack(uvm::uvm_packer& p) const
{
    p << addr << data << strb << (int)kind_e << valid;
}

void nmi_rw::do_unpack(uvm::uvm_packer& p)
{
    int rw;
    p >> addr >> data >> strb >> valid >> rw;
    kind_e = (nmi::nmi_rw_enum)rw;
}

void nmi_rw::do_copy(const uvm::uvm_object& rhs)
{
    const nmi_rw* drhs = dynamic_cast<const nmi_rw*>(&rhs);
    if (!drhs) {
        UVM_FATAL("DO_COPY", "Object not of type nmi_rw");
    }

    addr = drhs->addr;
    data = drhs->data;
    strb = drhs->strb;
    kind_e = drhs->kind_e;
    valid = drhs->valid;
}

bool nmi_rw::do_compare(const uvm_object& rhs) const
{
    const nmi_rw* drhs = dynamic_cast<const nmi_rw*>(&rhs);
    if (!drhs) {
        UVM_FATAL("DO_COPY", "Object not of type nmi_rw");
    }

    return (addr == drhs->addr && data == drhs->data && strb == drhs->strb && kind_e == drhs->kind_e && valid == drhs->valid);
}

std::string nmi_rw::convert2string() const
{
    std::ostringstream str;
    str << " addr: " << addr.to_uint();
    str << " data: " << data.to_uint();
    str << " strb: " << strb;
    str << " valid: " << valid;

    std::string kind_str;
    if(kind_e == nmi::READ)
        kind_str = "READ";
    else if(kind_e == nmi::WRITE)
        kind_str = "WRITE";
    else
        kind_str = "INSN";
        
    str << " read_write: " << kind_str;
    return str.str();
}
