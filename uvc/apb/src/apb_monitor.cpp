// #include "sct_assert.h"
#include <string>
#include <systemc>
#include <uvm>

#include "apb_monitor.h"
#include "apb_rw.h"
#include "apb_if.h"
#include "apb_agent.h"

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

apb_monitor::apb_monitor(uvm::uvm_component_name name) :
    uvm::uvm_monitor(name),
    sigs(nullptr)
{}

void apb_monitor::build_phase(uvm::uvm_phase & phase)
{
    uvm::uvm_monitor::build_phase(phase);

    apb_agent* agent = dynamic_cast<apb_agent*>(get_parent());
    if (agent) {
        sigs = agent->vif;
    }
    else {
        if (!uvm::uvm_config_db<Apb_if*>::get(this, "", "vif", sigs)) {
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

void apb_monitor::connect_phase(uvm::uvm_phase & phase){
    static_assertions();


}

void apb_monitor::static_assertions(){
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

void apb_monitor::dynamic_assertions(){
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


void apb_monitor::run_monitor(){
    apb_rw* tr;
    tr = apb_rw::type_id::create("tr", this);
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

void apb_monitor::run_phase(uvm::uvm_phase & phase)
{
    SC_FORK
        sc_core::sc_spawn(sc_core::sc_bind(&apb_monitor::run_monitor, this)),
        sc_core::sc_spawn(sc_core::sc_bind(&apb_monitor::dynamic_assertions, this)),
    SC_JOIN
}
