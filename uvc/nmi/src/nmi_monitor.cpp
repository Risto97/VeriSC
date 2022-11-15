// #include "sct_assert.h"
#include <string>
#include <systemc>
#include <uvm>

#include "nmi_monitor.h"
#include "nmi_rw.h"
#include "nmi_if.h"
#include "nmi_agent.h"


namespace uvc::nmi{
    static inline int check_wstrb(sc_dt::sc_uint<4> strb){
        if(strb == 0b0000 || strb == 0b1111 || strb == 0b1100 || strb == 0b0011 || 
           strb == 0b1000 || strb == 0b0100 || strb == 0b0010 || strb == 0b0001)
            return true;
        else
            return false;
        
    }

    monitor::monitor(uvm::uvm_component_name name) :
        uvm::uvm_monitor(name),
        sigs(nullptr)
    {


    }

    void monitor::build_phase(uvm::uvm_phase & phase)
    {
        uvm::uvm_monitor::build_phase(phase);

        agent* ag = dynamic_cast<agent*>(get_parent());
        if (ag) {
            sigs = ag->vif;
        }
        else {
            if (!uvm::uvm_config_db<intf*>::get(this, "", "vif", sigs)) {
                UVM_FATAL("NMI/MON/NOVIF", "No virtual interface specified for this monitor instance");
            }
        }

        mem_addr_old = new sc_core::sc_signal<sc_dt::sc_uint<32>>("addr_old");
        mem_instr_old = new sc_core::sc_signal<bool>("mem_instr_old");
        mem_wdata_old = new sc_core::sc_signal<sc_dt::sc_uint<32>>("mem_wdata_old");
        mem_wstrb_old = new sc_core::sc_signal<sc_dt::sc_uint<4>>("mem_wstrb_old");
        mem_valid_old = new sc_core::sc_signal<bool>("mem_valid_old");


    }

    void monitor::connect_phase(uvm::uvm_phase & phase){
        static_assertions();


    }

    void monitor::static_assertions(){
        // SCT_ASSERT(sigs->mem_valid , SCT_TIME(0), check_wstrb(sigs->mem_wstrb.read()) , sigs->clk->posedge_event());  
        // SCT_ASSERT(sigs->mem_instr , SCT_TIME(0), sigs->mem_wstrb.read() == 0 , sigs->clk->posedge_event());  
        // SCT_ASSERT((sigs->mem_valid && !sigs->mem_ready), SCT_TIME(1), ((sigs->mem_addr  == *mem_addr_old) &&
        //            (sigs->mem_instr == *mem_instr_old) && (sigs->mem_wdata == *mem_wdata_old) &&
        //            (sigs->mem_wstrb == *mem_wstrb_old)), sigs->clk->posedge_event());  
        // SCT_ASSERT(sigs->mem_valid && !sigs->mem_ready , SCT_TIME(1), sigs->mem_valid, sigs->clk->posedge_event());  
        //
        // SCT_ASSERT(sigs->mem_valid && !sigs->mem_ready, SCT_TIME(1), sigs->mem_addr  == *mem_addr_old, sigs->clk->posedge_event());  
        // SCT_ASSERT(sigs->mem_valid && !sigs->mem_ready, SCT_TIME(1), sigs->mem_instr == *mem_instr_old, sigs->clk->posedge_event());  
        // SCT_ASSERT(sigs->mem_valid && !sigs->mem_ready, SCT_TIME(1), sigs->mem_wdata == *mem_wdata_old, sigs->clk->posedge_event());  
        // SCT_ASSERT(sigs->mem_valid && !sigs->mem_ready, SCT_TIME(1), sigs->mem_wstrb == *mem_wstrb_old, sigs->clk->posedge_event());  


    }

    void monitor::dynamic_assertions(){
        while(1){
            // sc_time t;
            // auto val = sc_core::sc_get_curr_simcontext();
            // UVM_INFO("GET STATUS: ", "Status: " + std::to_string(val->get_status()), uvm::UVM_LOW);
            sc_core::wait(sigs->clk->posedge_event());
            mem_addr_old->write(sigs->mem_addr.read());
            mem_instr_old->write(sigs->mem_instr.read());
            mem_wdata_old->write(sigs->mem_wdata.read());
            mem_wstrb_old->write(sigs->mem_wstrb.read());
            mem_valid_old->write(sigs->mem_valid.read());
            // UVM_INFO("Valid: ", "old: " + std::to_string(mem_valid_old->read()) + " new " + std::to_string(sigs->mem_valid.read()) + " Assert: " + std::to_string((*mem_valid_old && sigs->mem_valid && !sigs->mem_ready) && (sigs->mem_wdata != *mem_wdata_old)), uvm::UVM_LOW);
            // UVM_INFO("wdata: ", "old: " + std::to_string(mem_wdata_old->read()) + " new " + std::to_string(sigs->mem_wdata.read()), uvm::UVM_LOW);
            // UVM_INFO("GET STATUS: ", "Status: " + std::to_string(val->get_status()), uvm::UVM_LOW);
            // val->next_time(t);
            // UVM_INFO("GET Next time: ", "next time: " + t.to_string(), uvm::UVM_LOW);
        }
    }

    void monitor::run_monitor(){
        while (1) {
            rw* tr;
            do {
                sc_core::wait(sigs->clk->posedge_event());
            }
            while ( !(sigs->mem_valid == 1 &&
                      sigs->mem_ready == 1));

            tr = rw::type_id::create("tr", this);

            tr->addr = sigs->mem_addr;
            tr->strb = sigs->mem_wstrb;
            tr->valid = true;

            if(sigs->mem_instr == 1){
                tr->kind_e = INSN;
                tr->data = sigs->mem_rdata;
            }
            else{
                if(sigs->mem_wstrb.read() != 0){
                    tr->kind_e = WRITE;
                    tr->data = sigs->mem_wdata;
                }
                else{
                    tr->kind_e = READ;
                    tr->data = sigs->mem_rdata;
                }
            }

            ap.write(*tr);
        }
    }


    void monitor::run_phase(uvm::uvm_phase & phase)
    {
        SC_FORK
            sc_core::sc_spawn(sc_core::sc_bind(&monitor::run_monitor, this)),
            sc_core::sc_spawn(sc_core::sc_bind(&monitor::dynamic_assertions, this)),
        SC_JOIN
    }
}
