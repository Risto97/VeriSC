#ifndef NMI_IF_H_
#define NMI_IF_H_

#include "sysc/kernel/sc_event.h"
#include <systemc>
#include <uvm>

class nmi_if : public sc_core::sc_module {
    public:
        sc_core::sc_clock  *clk;
        sc_core::sc_signal<bool>  mem_valid;
        sc_core::sc_signal<bool>  mem_instr;
        sc_core::sc_signal<bool>  mem_ready;

        sc_core::sc_signal<sc_dt::sc_uint<32>> mem_addr;
        sc_core::sc_signal<sc_dt::sc_uint<32>> mem_wdata;
        sc_core::sc_signal<sc_dt::sc_uint<4>> mem_wstrb;
        sc_core::sc_signal<sc_dt::sc_uint<32>> mem_rdata;

        nmi_if(const sc_core::sc_module_name & name, sc_core::sc_clock *clk_in) :
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

};

#endif /* NMI_IF_H_ */