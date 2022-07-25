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

#include <systemc>
#include <uvm>
#include <string>
#include <stdint.h>

#include "apb_driver.h"
#include "apb_rw.h"
#include "apb_if.h"
#include "apb_agent.h"
#include "apb_modes.h"

apb_driver::apb_driver(uvm::uvm_component_name name) :
    uvm::uvm_driver<apb_rw>(name),
    sigs(nullptr)
{}

void apb_driver::build_phase(uvm::uvm_phase & phase)
{
    UVM_INFO("APB DRIVER build PHASE", "", uvm::UVM_MEDIUM);
    
    uvm::uvm_driver<apb_rw>::build_phase(phase);

    apb_agent* agent = dynamic_cast<apb_agent*>(get_parent());
    if (agent) {
        sigs = agent->vif;
        mode = agent->mode;
    }
    else {
        if (!uvm::uvm_config_db<apb_if*>::get(this, "", "vif", sigs)) {
            UVM_FATAL("APB/DRV/NOVIF", "No virtual interface specified for this driver instance");
        }
        if(!uvm::uvm_config_db<apb::mode_t>::get(this, "", "mode", mode)){
            UVM_FATAL("APB", "No mode defined, aborting simulation");
        }
    }
}

void apb_driver::run_phase(uvm::uvm_phase & phase)
{

    if(mode == apb::MASTER){
        UVM_INFO("CONFIGURED AS MASTER", "", uvm::UVM_MEDIUM);
        this->sigs->psel = 0;
        this->sigs->penable = 0;

        while (1) {
            apb_rw req;
            sc_core::wait(this->sigs->pclk->posedge_event());

            this->seq_item_port->get_next_item(req);

            sc_core::wait(this->sigs->pclk->posedge_event());

            switch (req.kind_e) {
                case READ : this->read(req.addr, req.data, req.strb, req.valid); break;
                case WRITE : this->write(req.addr, req.data, req.strb, req.valid); break;
            }

            this->seq_item_port->item_done();
            this->trig.notify();
        }
    }
}

void apb_driver::read(const sc_dt::sc_lv<32> & addr, sc_dt::sc_lv<32> & data, const sc_dt::sc_lv<4> & strb, const bool valid)
{
    this->sigs->paddr = addr;
    this->sigs->pwrite = 0;
    this->sigs->pstrb = strb;
    this->sigs->psel = valid;
    sc_core::wait(this->sigs->pclk->posedge_event());
    this->sigs->penable = valid;
    sc_core::wait(this->sigs->pclk->posedge_event());
    data = this->sigs->prdata;
    this->sigs->psel = 0;
    this->sigs->penable = 0;
}

void apb_driver::write(const sc_dt::sc_lv<32> & addr, const sc_dt::sc_lv<32> & data, const sc_dt::sc_lv<4> & strb, const bool valid)
{
    this->sigs->paddr = addr;
    this->sigs->pwdata = data;
    this->sigs->pstrb = strb;
    this->sigs->pwrite = 1;
    this->sigs->psel = valid;
    sc_core::wait(this->sigs->pclk->posedge_event());
    this->sigs->penable = valid;
    sc_core::wait(this->sigs->pclk->posedge_event());
    this->sigs->psel = 0;
    this->sigs->penable = 0;
}
