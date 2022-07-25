// 
// -------------------------------------------------------------
//    Copyright 2021 NXP B.V.
//    Copyright 2004-2011 Synopsys, Inc.
//    Copyright 2010 Mentor Graphics Corporation
//    Copyright 2010 Cadence Design Systems, Inc.
//    All Rights Reserved Worldwide
// 
//    Licensed under the Apache License, Version 2.0 (the
//    "License"); you may not use this file except in
//    compliance with the License.  You may obtain a copy of
//    the License at
// 
//        http://www.apache.org/licenses/LICENSE-2.0
// 
//    Unless required by applicable law or agreed to in
//    writing, software distributed under the License is
//    distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
//    CONDITIONS OF ANY KIND, either express or implied.  See
//    the License for the specific language governing
//    permissions and limitations under the License.
// -------------------------------------------------------------
// 

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
}

void apb_rw::do_pack(uvm::uvm_packer& p) const
{
    p << addr << data << strb << (int)kind_e << valid;
}

void apb_rw::do_unpack(uvm::uvm_packer& p)
{
    int rw;
    p >> addr >> data >> strb >> valid >> rw;
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
}

bool apb_rw::do_compare(const uvm_object& rhs) const
{
    const apb_rw* drhs = dynamic_cast<const apb_rw*>(&rhs);
    if (!drhs) {
        UVM_FATAL("DO_COPY", "Object not of type apb_rw");
    }

    return (addr == drhs->addr && data == drhs->data && strb == drhs->strb && kind_e == drhs->kind_e && valid == drhs->valid);
}

std::string apb_rw::convert2string() const
{
    std::ostringstream str;
    str << " addr: " << addr.to_uint();
    str << " data: " << data.to_uint();
    str << " strb: " << strb;
    str << " read_write: " << (kind_e == READ ? "READ" : "WRITE");
    str << " valid: " << valid;
    return str.str();
}
