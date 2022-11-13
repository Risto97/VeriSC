#include "i2c_if.h"
#include <string>
#include <sysc/kernel/sc_simcontext.h>
#include <systemc.h>

void i2c_if::wait_posedge_rv(sc_core::sc_signal_rv<1> &s) {
  sc_dt::sc_lv<1> old_val;
  do {
    old_val = s.read();
    sc_core::wait(s.value_changed_event());
  } while (s.read() == 0 || s.read() == "x" || old_val == 1);
}

void i2c_if::wait_negedge_rv(sc_core::sc_signal_rv<1> &s) {
  do {
    sc_core::wait(s.value_changed_event());
  } while (s.read() != 0);
}

bool i2c_if::read_rv(sc_core::sc_signal_rv<1> &s) {
  if (s.read() == "z")
    return true;
  else if (s.read() == 1)
    return true;
  else if (s.read() == 0)
    return false;
  return false;
}

bool i2c_if::read_rv_sda() { return read_rv(sda); };
bool i2c_if::read_rv_scl() { return read_rv(scl); };

bool i2c_if::wait_start_condition() {
  wait_negedge_rv(sda);
  if (read_rv(scl) == 1)
    return true;
  return false;
}

uint32_t i2c_if::read_addr() {
  uint32_t addr = 0;
  for (int i = 0; i < 7; i++) {
    wait_posedge_rv(scl);
    addr |= read_rv(sda) << (6 - i);
  }

  return addr;
}

bool i2c_if::read_rw() {
  wait_posedge_rv(scl);
  return read_rv(sda);
}

bool i2c_if::read_ack() {
  wait_posedge_rv(scl);
  return !read_rv(sda); //! sda_i; // PLARITY???
}

void i2c_if::drive_ack(const bool ack) {
  sc_core::wait(scl_negedge_e);
  sc_core::wait(1, sc_core::SC_NS); // change sda on stable low scl
  sda.write(0);            // POLARITY??
  sc_core::wait(scl_negedge_e);
  sc_core::wait(1, sc_core::SC_NS); /// change sda on stable low scl
  sda.write("z");                   // POLARITY??//  slave deassert acknowledge
}

int i2c_if::read_data_byte() {
  uint32_t data = 0;
  sc_core::wait(scl_posedge_e);
  data |= read_rv(sda) << (7);

  sc_core::wait(scl_posedge_e | stop_e);
  if (stop_e.triggered()) {
    return -1;
  }
  data |= read_rv(sda) << (6);

  for (int i = 2; i < 8; i++) {
    wait_posedge_rv(scl);
    data |= read_rv(sda) << (7 - i);
  }
  return data;
}

void i2c_if::drive_data_byte(const uint32_t data) {
  for (int i = 0; i < 8; i++) {
    sc_core::wait(1, sc_core::SC_NS); // change sda on stable low scl
    if ((data >> (7 - i)) & 1)
      sda.write("z");
    else
      sda.write(0);
    wait_negedge_rv(scl);
  }
  sda.write("z");
}

bool i2c_if::wait_stop_condition() {
  wait_posedge_rv(scl);
  wait_posedge_rv(sda);
  return read_rv(scl);
}
