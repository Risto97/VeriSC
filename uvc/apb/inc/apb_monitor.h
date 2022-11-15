#ifndef APB_MONITOR_H_
#define APB_MONITOR_H_

#include <systemc>
#include <uvm>

#include "apb_if.h"
#include "apb_rw.h"

namespace uvc::apb{
    class monitor : public uvm::uvm_monitor
    {
        public:
            Intf* sigs;

            sc_core::sc_signal<sc_dt::sc_uint<32>> *paddr_old;
            sc_core::sc_signal<bool>  *psel_old;
            sc_core::sc_signal<bool>  *penable_old;
            sc_core::sc_signal<bool>  *pwrite_old;
            sc_core::sc_signal<bool>  *pready_old;
            sc_core::sc_signal<sc_dt::sc_uint<32>> *prdata_old;
            sc_core::sc_signal<sc_dt::sc_uint<4>> *pstrb_old;
            sc_core::sc_signal<sc_dt::sc_uint<32>> *pwdata_old;
            //
            uvm::uvm_analysis_port<rw> ap;

            UVM_COMPONENT_UTILS(monitor);

            monitor(uvm::uvm_component_name name);

            virtual void static_assertions();
            virtual void dynamic_assertions();

            virtual void build_phase(uvm::uvm_phase & phase);

            virtual void connect_phase(uvm::uvm_phase & phase);

            virtual void run_phase(uvm::uvm_phase & phase);

            virtual void run_monitor();
    };
}

#endif /* APB_MONITOR_H_ */
