#ifndef I2C_DRIVER_H_
#define I2C_DRIVER_H_

#include <systemc>
#include <uvm>

#include "i2c_if.h"
#include "i2c_modes.h"
#include "i2c_rw.h"
#include <stdint.h>

namespace uvc::i2c{
class driver : public uvm::uvm_driver<rw> {
public:
  UVM_COMPONENT_UTILS(driver);

  intf *sigs;

  mode_t mode;

  driver(uvm::uvm_component_name name);

  virtual void build_phase(uvm::uvm_phase &phase);

  virtual void run_phase(uvm::uvm_phase &phase);

private:
  bool slave_drive_transaction();
  rw read(const rw &req);
  void write(const rw &req);
  rw put_rsp_get_req(rw &rsp, rw &req, const std::string msg);
  void put_rsp(rw &rsp, rw &req, const std::string msg);
  rw handle_read_from_slave(rw &rd_req);
  rw handle_write_to_slave(rw &req);
};
}

#endif
