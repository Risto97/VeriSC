#ifndef I2C_IF_H_
#define I2C_IF_H_

#include <sysc/communication/sc_signal_resolved.h>
#include <sysc/kernel/sc_event.h>
#include <sysc/kernel/sc_module.h>
#include <sysc/kernel/sc_simcontext.h>
#include <systemc>
#include <uvm>

#include "i2c_rw.h"

namespace uvc::i2c{
    class intf : public sc_core::sc_module {
    public:
      sc_core::sc_signal_rv<1> scl;
      sc_core::sc_signal_rv<1> sda;

      // Not a real stop condition because of delta cycles,
      // SDA can be asserted delta time after SCL
      sc_core::sc_event stop_e;
      sc_core::sc_event scl_posedge_e;
      sc_core::sc_event scl_negedge_e;
      sc_core::sc_event sda_posedge_e;
      sc_core::sc_event sda_negedge_e;

      // intf(const sc_core::sc_module_name &name)
      //     : sc_module(name), scl("scl"), sda("sda") {

      //   SC_METHOD(stop_cond_thread);
      //   sensitive << scl << sda;
      // }

      SC_CTOR(intf) {

        SC_THREAD(stop_cond_thread);
        SC_THREAD(scl_posedge_thread);
        SC_THREAD(scl_negedge_thread);
        SC_THREAD(sda_negedge_thread);
        SC_THREAD(sda_posedge_thread);
        // sensitive << scl << sda;
      }

      bool read_rv_sda();
      bool read_rv_scl();
      bool read_rv(sc_core::sc_signal_rv<1> &s);

      void wait_posedge_rv(sc_core::sc_signal_rv<1> &s);
      void wait_negedge_rv(sc_core::sc_signal_rv<1> &s);

      bool wait_start_condition();
      uint32_t read_addr();
      bool read_rw();
      bool read_ack();
      void drive_ack(const bool ack);
      int read_data_byte();
      bool wait_stop_condition();
      void drive_data_byte(const uint32_t data);
      // i2c_rw monitor_transaction();

    private:
      void stop_cond_thread() {
        while (true) {
          wait_posedge_rv(sda);
          if (read_rv(scl) == 1) {
            stop_e.notify();

            std::cout << "Stop Condition: " << read_rv(scl) << " "
                      << sc_core::sc_time_stamp() << "\n";
          }
        }
      }

      void scl_posedge_thread() {
        while (true) {
          wait_posedge_rv(scl);
          scl_posedge_e.notify();
        }
      }

      void scl_negedge_thread() {
        while (true) {
          wait_negedge_rv(scl);
          scl_negedge_e.notify();
        }
      }

      void sda_negedge_thread() {
        while (true) {
          wait_negedge_rv(sda);
          sda_negedge_e.notify();
        }
      }

      void sda_posedge_thread() {
        while (true) {
          wait_posedge_rv(sda);
          sda_posedge_e.notify();
        }
      }
    };
}

#endif /* I2C_IF_H_ */
