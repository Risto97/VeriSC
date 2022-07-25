// 
// -------------------------------------------------------------
//    Copyright 2021 NXP B.V.
//    Copyright 2004-2011 Synopsys, Inc.
//    Copyright 2010 Mentor Graphics Corporation
//    Copyright 2010-2011 Cadence Design Systems, Inc.
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


#ifndef APB_DRIVER_H_
#define APB_DRIVER_H_

#include <systemc>
#include <uvm>

#include "apb_rw.h"
#include "apb_if.h"
#include "apb_modes.h"
#include <stdint.h>
// #include "apb_agent.h"

// namespace apb{
//     enum mode_t { MASTER = 0, SLAVE = 1 };
// }

class apb_driver : public uvm::uvm_driver<apb_rw>
{
    public:
        UVM_COMPONENT_UTILS(apb_driver);

        sc_core::sc_event trig;
        apb_if* sigs;

        apb::mode_t mode;


        apb_driver(uvm::uvm_component_name name);

        virtual void build_phase(uvm::uvm_phase & phase);

        virtual void run_phase(uvm::uvm_phase & phase);

    private:
        void read(const sc_dt::sc_lv<32> & addr, sc_dt::sc_lv<32> & data, const sc_dt::sc_lv<4> & strb, const bool valid);
        void write(const sc_dt::sc_lv<32> & addr, const sc_dt::sc_lv<32> & data, const sc_dt::sc_lv<4> & strb, const bool valid);
};

#endif /* APB_DRIVER_H_ */
