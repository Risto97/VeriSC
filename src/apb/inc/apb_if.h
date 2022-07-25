#ifndef APB_IF_H_
#define APB_IF_H_

#include <systemc>
#include <uvm>

class apb_if : public sc_core::sc_module {
    public:
        sc_core::sc_clock  *pclk;
        sc_core::sc_signal<sc_dt::sc_uint<32>> paddr;
        sc_core::sc_signal<bool>  psel;
        sc_core::sc_signal<bool>  penable;
        sc_core::sc_signal<bool>  pwrite;
        sc_core::sc_signal<bool>  pready;
        sc_core::sc_signal<sc_dt::sc_uint<32>> prdata;
        sc_core::sc_signal<sc_dt::sc_uint<4>> pstrb;
        sc_core::sc_signal<sc_dt::sc_uint<32>> pwdata;

        apb_if(const sc_core::sc_module_name & name, sc_core::sc_clock *clk_in) :
            sc_module(name),
            paddr("paddr"),
            psel("psel"),
            penable("penable"),
            pready("pready"),
            pwrite("pwrite"),
            prdata("prdata"),
            pstrb("pstrb"),
            pwdata("pwdata") {
        pclk = clk_in;

    }

};

#endif /* APB_IF_H_ */
