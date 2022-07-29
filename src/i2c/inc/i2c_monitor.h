#ifndef I2C_MONITOR_H_
#define I2C_MONITOR_H_

#include <systemc>
#include <uvm>

#include "i2c_if.h"
#include "i2c_rw.h"

class i2c_monitor : public uvm::uvm_monitor {
public:
  i2c_if *sigs;

  //
  uvm::uvm_analysis_port<i2c_rw> ap;

  UVM_COMPONENT_UTILS(i2c_monitor);

  i2c_monitor(uvm::uvm_component_name name);

  virtual void static_assertions();
  virtual void dynamic_assertions();

  virtual void build_phase(uvm::uvm_phase &phase);

  virtual void connect_phase(uvm::uvm_phase &phase);

  virtual void run_phase(uvm::uvm_phase &phase);

  virtual void run_monitor();
  i2c_rw monitor_transaction();
  bool wait_start_condition();
  uint32_t read_addr();
  bool read_rw();
  bool read_ack();
  uint32_t read_data_byte();
  bool wait_stop_condition();
};

#endif
