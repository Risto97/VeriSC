#ifndef NMI_MONITOR_H_
#define NMI_MONITOR_H_

#include <systemc>
#include <uvm>
// #include "sct_assert.h"

#include "nmi_if.h"
#include "nmi_rw.h"
#include "sysc/kernel/sc_event.h"

namespace uvc::nmi{
    class monitor : public uvm::uvm_monitor
    {
        public:
            intf* sigs;

            sc_core::sc_signal<sc_dt::sc_uint<32>> *mem_addr_old;
            sc_core::sc_signal<bool> *mem_instr_old;
            sc_core::sc_signal<bool> *mem_valid_old;
            sc_core::sc_signal<sc_dt::sc_uint<32>> *mem_wdata_old;
            sc_core::sc_signal<sc_dt::sc_uint<4>> *mem_wstrb_old;

            uvm::uvm_analysis_port<rw> ap;

            UVM_COMPONENT_UTILS(monitor);

            monitor(uvm::uvm_component_name name);

            // MyMod assert_mod{"assert_mod"};

            virtual void static_assertions();
            virtual void dynamic_assertions();

            virtual void run_monitor();

            virtual void build_phase(uvm::uvm_phase & phase);

            virtual void connect_phase(uvm::uvm_phase & phase);

            virtual void run_phase(uvm::uvm_phase & phase);

    };
}

#endif /* NMI_MONITOR_H_ */
