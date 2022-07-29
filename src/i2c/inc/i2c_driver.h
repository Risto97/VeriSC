#ifndef I2C_DRIVER_H_
#define I2C_DRIVER_H_

#include <systemc>
#include <uvm>

#include "i2c_if.h"
#include "i2c_modes.h"
#include "i2c_rw.h"
#include <stdint.h>

class i2c_driver : public uvm::uvm_driver<i2c_rw> {
public:
  UVM_COMPONENT_UTILS(i2c_driver);

  i2c_if *sigs;

  i2c::mode_t mode;

  i2c_driver(uvm::uvm_component_name name);

  virtual void build_phase(uvm::uvm_phase &phase);

  virtual void run_phase(uvm::uvm_phase &phase);

private:
  bool slave_drive_transaction();
  i2c_rw read(const i2c_rw &req);
  void write(const i2c_rw &req);
  i2c_rw put_rsp_get_req(i2c_rw &rsp, i2c_rw &req, const std::string msg);
  void put_rsp(i2c_rw &rsp, i2c_rw &req, const std::string msg);
  i2c_rw handle_read_from_slave(i2c_rw &rd_req);
  i2c_rw handle_write_to_slave(i2c_rw &req);
};

#endif
