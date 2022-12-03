#ifndef NMI_IF_H_
#define NMI_IF_H_

#include "sysc/kernel/sc_event.h"
#include <systemc>
#include <uvm>

namespace uvc::nmi{
    class intf : public sc_core::sc_module {
        public:
            sc_core::sc_clock  *clk;
            sc_core::sc_signal<bool>  mem_valid;
            sc_core::sc_signal<bool>  mem_instr;
            sc_core::sc_signal<bool>  mem_ready;

            sc_core::sc_signal<sc_dt::sc_uint<32>> mem_addr;
            sc_core::sc_signal<sc_dt::sc_uint<32>> mem_wdata;
            sc_core::sc_signal<sc_dt::sc_uint<4>> mem_wstrb;
            sc_core::sc_signal<sc_dt::sc_uint<32>> mem_rdata;

            intf(const sc_core::sc_module_name & name, sc_core::sc_clock *clk_in) :
                sc_module(name),
                mem_valid("mem_valid"),
                mem_instr("mem_instr"),
                mem_ready("mem_ready"),
                mem_addr("mem_addr"),
                mem_wdata("mem_wdata"),
                mem_wstrb("mem_wstrb"),
                mem_rdata("mem_rdata") 
                {
            clk = clk_in;
            }

    void trace(sc_core::sc_trace_file *wf){
        std::string name = this->name();
        sc_trace(wf, mem_valid, name + "_mem_valid");
        sc_trace(wf, mem_ready, name + "_mem_ready");
        sc_trace(wf, mem_addr,  name + "_mem_addr");
        sc_trace(wf, mem_wstrb, name + "_mem_wstrb");
        sc_trace(wf, mem_rdata, name + "_mem_rdata");
        sc_trace(wf, mem_wdata, name + "_mem_wdata");
        sc_trace(wf, mem_instr, name + "_mem_instr");

    }

    };
}

#endif /* NMI_IF_H_ */
