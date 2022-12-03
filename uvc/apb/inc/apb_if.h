#ifndef APB_IF_H_
#define APB_IF_H_

#include <systemc>
#include <uvm>


namespace uvc::apb{
    template<uint32_t DATA_WIDTH=32, uint32_t ADDR_WIDTH=32>
    class intf : public sc_core::sc_module {
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

            intf(const sc_core::sc_module_name & name, sc_core::sc_clock *clk_in) :
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

    void trace(sc_core::sc_trace_file *wf){
        std::string name = this->name();
        sc_trace(wf, paddr, name + "_paddr");
        sc_trace(wf, psel, name + "_psel");
        sc_trace(wf, penable,  name + "_penable");
        sc_trace(wf, pwrite, name + "_pwrite");
        sc_trace(wf, pready, name + "_pready");
        sc_trace(wf, prdata, name + "_prdata");
        sc_trace(wf, pstrb, name + "_pstrb");
        sc_trace(wf, pwdata, name + "_pwdata");
        sc_trace(wf, pslverr, name + "_pslverr");
    }

    };

    using Intf = intf<32, 32>;
}

#endif /* APB_IF_H_ */
