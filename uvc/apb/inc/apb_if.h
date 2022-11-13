#ifndef APB_IF_H_
#define APB_IF_H_

#include <systemc>
#include <uvm>


template<uint32_t DATA_WIDTH=32, uint32_t ADDR_WIDTH=32>
class apb_if : public sc_core::sc_module {
    public:
        sc_core::sc_clock  *pclk;
        sc_core::sc_signal<sc_dt::sc_uint<ADDR_WIDTH>> paddr;
        sc_core::sc_signal<bool>  psel;
        sc_core::sc_signal<bool>  penable;
        sc_core::sc_signal<bool>  pwrite;
        sc_core::sc_signal<bool>  pready;
        sc_core::sc_signal<sc_dt::sc_uint<DATA_WIDTH>> prdata;
        sc_core::sc_signal<sc_dt::sc_uint<DATA_WIDTH/8>> pstrb;
        sc_core::sc_signal<sc_dt::sc_uint<DATA_WIDTH>> pwdata;
        sc_core::sc_signal<bool>  pslverr;

        apb_if(const sc_core::sc_module_name & name, sc_core::sc_clock *clk_in) :
            sc_module(name),
            paddr("paddr"),
            psel("psel"),
            penable("penable"),
            pready("pready"),
            pwrite("pwrite"),
            prdata("prdata"),
            pstrb("pstrb"),
            pwdata("pwdata"),
            pslverr("pslverr"){
        pclk = clk_in;

    }

};

using Apb_if = apb_if<32, 32>;

#endif /* APB_IF_H_ */
