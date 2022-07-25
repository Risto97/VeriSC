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


#ifndef APB_IF_H_
#define APB_IF_H_

#include <systemc>
#include <uvm>

class apb_if : public sc_core::sc_module {
    public:
        sc_core::sc_clock  *pclk;
        sc_core::sc_signal<sc_dt::sc_uint<32>> paddr;
        sc_core::sc_signal<bool>  psel;
        sc_core::sc_signal<bool>  penable;
        sc_core::sc_signal<bool>  pwrite;
        sc_core::sc_signal<bool>  pready;
        sc_core::sc_signal<sc_dt::sc_uint<32>> prdata;
        sc_core::sc_signal<sc_dt::sc_uint<4>> pstrb;
        sc_core::sc_signal<sc_dt::sc_uint<32>> pwdata;

        apb_if(const sc_core::sc_module_name & name, sc_core::sc_clock *clk_in) :
            sc_module(name),
            paddr("paddr"),
            psel("psel"),
            penable("penable"),
            pready("pready"),
            pwrite("pwrite"),
            prdata("prdata"),
            pstrb("pstrb"),
            pwdata("pwdata") {
        pclk = clk_in;

    }

};

#endif /* APB_IF_H_ */
