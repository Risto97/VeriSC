// #include "sct_assert.h"
#include <string>
#include <systemc>
#include <uvm>

#include "apb_monitor.h"
#include "apb_rw.h"
#include "apb_if.h"
#include "apb_agent.h"

namespace uvc::apb{
    template <class Type>
    inline bool check_onehot(Type bits)
    {
        return bits && !(bits & (bits-1));
    }

    template <class Type>
    inline bool check_onehot_or_zero(Type bits)
    {
        if(bits == 0)
            return true;
        else
            return check_onehot(bits);
    }

    monitor::monitor(uvm::uvm_component_name name) :
        uvm::uvm_monitor(name),
        sigs(nullptr)
    {}

    void monitor::build_phase(uvm::uvm_phase & phase)
    {
        uvm::uvm_monitor::build_phase(phase);

        agent* ag = dynamic_cast<agent*>(get_parent());
        if (ag) {
            sigs = ag->vif;
        }
        else {
            if (!uvm::uvm_config_db<Intf*>::get(this, "", "vif", sigs)) {
                UVM_FATAL("APB/MON/NOVIF", "No virtual interface specified for this monitor instance");
            }
        }




        paddr_old = new sc_core::sc_signal<sc_dt::sc_uint<32>>("paddr_old");
        psel_old = new sc_core::sc_signal<bool>("psel_old");
        penable_old = new sc_core::sc_signal<bool>("penable_old");
        pwrite_old = new sc_core::sc_signal<bool>("pwrite_old");
        pready_old = new sc_core::sc_signal<bool>("pready_old");
        prdata_old = new sc_core::sc_signal<sc_dt::sc_uint<32>>("prdata_old");
        pstrb_old = new sc_core::sc_signal<sc_dt::sc_uint<4>>("pstrb_old");
        pwdata_old = new sc_core::sc_signal<sc_dt::sc_uint<32>>("pwdata_old");
    }

    void monitor::connect_phase(uvm::uvm_phase & phase){
        static_assertions();


    }

    void monitor::static_assertions(){
        // SCT_ASSERT(!*psel_old && sigs->psel, SCT_TIME(1), sigs->penable, sigs->pclk->posedge_event());
        // SCT_ASSERT(sigs->psel && !sigs->pready, SCT_TIME(1), sigs->psel, sigs->pclk->posedge_event());
        // SCT_ASSERT(sigs->penable && !sigs->pready, SCT_TIME(1), sigs->penable, sigs->pclk->posedge_event());
        // SCT_ASSERT(!sigs->pwrite && sigs->psel, SCT_TIME(0), sigs->pstrb.read() == 0, sigs->pclk->posedge_event());
        //
        // SCT_ASSERT(sigs->psel && !sigs->pready, SCT_TIME(1), (sigs->paddr == *paddr_old) && 
        //           (sigs->pwrite == *pwrite_old) && (sigs->pstrb == *pstrb_old) &&
        //           (sigs->pwdata == *pwdata_old), sigs->pclk->posedge_event());
        //
        // SCT_ASSERT(true, SCT_TIME(0), check_onehot_or_zero(sigs->psel.read()), sigs->pclk->posedge_event());
        // SCT_ASSERT(true, SCT_TIME(0), check_onehot_or_zero(sigs->penable.read()), sigs->pclk->posedge_event());
        //
    }

    void monitor::dynamic_assertions(){
        while(1){
            sc_core::wait(sigs->pclk->posedge_event());
            paddr_old->write(sigs->paddr.read());
            psel_old->write(sigs->psel.read());
            penable_old->write(sigs->penable.read());
            pwrite_old->write(sigs->pwrite.read());
            pready_old->write(sigs->pready.read());
            prdata_old->write(sigs->prdata.read());
            pstrb_old->write(sigs->pstrb.read());
            pwdata_old->write(sigs->pwdata.read());
        }
    }


    void monitor::run_monitor(){
        rw* tr;
        tr = rw::type_id::create("tr", this);
        while (1) {
            do{
                sc_core::wait(this->sigs->pclk->posedge_event());
            }
            while (sigs->psel == 0 || sigs->penable == 0);

            tr->kind_e = (sigs->pwrite == 1) ? WRITE : READ;
            tr->addr = sigs->paddr;
            tr->strb = sigs->pstrb;
            tr->slv_err = sigs->pslverr;
            tr->valid = true;

            tr->data = (tr->kind_e == READ) ? sigs->prdata : sigs->pwdata;
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
