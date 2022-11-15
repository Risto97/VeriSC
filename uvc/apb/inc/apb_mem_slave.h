#ifndef _APB_MEM_SLAVE_H_
#define _APB_MEM_SLAVE_H_

#include "systemc.h"
#include <algorithm>
#include <iostream>

namespace uvc::apb{
    static sc_dt::sc_uint<32> strobe_to_mask(sc_dt::sc_uint<4> strb){
        sc_dt::sc_uint<32> mask = 0;
        for(int i = 0; i < strb.length(); i++){
            if(strb.bit(i) == 1){
                mask = mask | (0xff << i*8);

            }

        }
        return mask;
    }

    SC_MODULE(mem_slave) {

      sc_core::sc_in<bool> pclk{"PCLK"};
      sc_core::sc_in<sc_dt::sc_uint<32>> paddr{"PADDR"};
      sc_core::sc_in<bool> psel{"PSEL"};
      sc_core::sc_in<bool> penable{"PENABLE"};
      sc_core::sc_in<bool> pwrite{"PWRITE"};
      sc_core::sc_out<bool> pready{"PREADY"};
      sc_core::sc_out<sc_dt::sc_uint<32>> prdata{"PRDATA"};
      sc_core::sc_in<sc_dt::sc_uint<4>> pstrb{"PSTRB"};
      sc_core::sc_in<sc_dt::sc_uint<32>> pwdata{"PWDATA"};

      uint32_t mem[1024];
      void read_method() {
        pready = 1;
        prdata.write(mem[paddr.read()]);
      }

      void write_thread() {
        while (true) {
          wait();
          if (penable == 1 && psel == 1) {
            mem[paddr.read()] = pwdata.read() & strobe_to_mask(pstrb.read());
          }
        }
      }

      SC_CTOR(mem_slave) {

        std::fill_n(mem, 1024, -1);

        // write function
        SC_CTHREAD(write_thread, pclk.pos());

        // read function
        SC_METHOD(read_method);
        sensitive << paddr;
      }
    };
}

#endif
