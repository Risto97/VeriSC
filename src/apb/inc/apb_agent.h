// 
// -------------------------------------------------------------
//    Copyright 2021 NXP B.V.
//    Copyright 2004-2011 Synopsys, Inc.
//    Copyright 2010-2011 Mentor Graphics Corporation
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

#ifndef APB_AGENT_H_
#define APB_AGENT_H_

#include <systemc>
#include <uvm>

#include "apb.h"
#include "apb_sequencer.h"
#include "apb_driver.h"
#include "apb_monitor.h"
#include "apb_modes.h"


class apb_agent : public uvm::uvm_agent
{
    public:
        apb_sequencer* sqr;
        apb_driver* drv;
        apb_monitor* mon;

        apb::mode_t mode;

        apb_if* vif;

        UVM_COMPONENT_UTILS(apb_agent);

        apb_agent(uvm::uvm_component_name name);
        ~apb_agent();

        virtual void build_phase(uvm::uvm_phase& phase);

        virtual void connect_phase(uvm::uvm_phase& phase);

};

#endif /* APB_AGENT_H */
