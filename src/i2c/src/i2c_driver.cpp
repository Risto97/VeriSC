#include <stdint.h>
#include <string>
#include <sysc/kernel/sc_wait.h>
#include <systemc>
#include <uvm>

#include "i2c_agent.h"
#include "i2c_driver.h"
#include "i2c_if.h"
#include "i2c_rw.h"

i2c_driver::i2c_driver(uvm::uvm_component_name name)
    : uvm::uvm_driver<i2c_rw>(name), sigs(nullptr) {}

void i2c_driver::build_phase(uvm::uvm_phase &phase) {
  UVM_INFO("i2c DRIVER build PHASE", "", uvm::UVM_MEDIUM);

  uvm::uvm_driver<i2c_rw>::build_phase(phase);

  i2c_agent *agent = dynamic_cast<i2c_agent *>(get_parent());

  UVM_INFO("i2c DRIVER build PHASE2", "", uvm::UVM_MEDIUM);
  if (agent) {
    sigs = agent->vif;
    mode = agent->mode;
  } else {
    if (!uvm::uvm_config_db<i2c_if *>::get(this, "", "vif", sigs)) {
      UVM_FATAL("i2c/DRV/NOVIF",
                "No virtual interface specified for this driver instance");
    }
    if (!uvm::uvm_config_db<i2c::mode_t>::get(this, "", "mode", mode)) {
      UVM_FATAL("i2c", "No mode defined, aborting simulation");
    }
  }
}

i2c_rw i2c_driver::put_rsp_get_req(i2c_rw &rsp, i2c_rw &req,
                                   const std::string msg) {
  rsp.set_id_info(req);
  rsp.set_name(msg.c_str());
  seq_item_port.put_response(rsp);
  seq_item_port->item_done();
  seq_item_port->get_next_item(req);
  return req;
}

void i2c_driver::put_rsp(i2c_rw &rsp, i2c_rw &req, const std::string msg) {
  rsp.set_id_info(req);
  rsp.set_name(msg.c_str());
  seq_item_port->item_done();
  seq_item_port.put_response(rsp);
}

i2c_rw i2c_driver::handle_read_from_slave(i2c_rw &rd_req) {
  i2c_rw rsp, req;
  bool ack;
  UVM_INFO("Driver I2c", "Doing a read", uvm::UVM_LOW);
  rsp.kind_e = i2c::READ;
  rsp.addr = 0;
  rsp.valid = false;

  req = put_rsp_get_req(rsp, rd_req, "Dummy response, get data");
  req.print();

  if (!req.valid) {
    UVM_ERROR("I2C Slave", "Slave didnt send data");
    rsp.error = true;
    return rsp;
  }
  sigs->drive_data_byte(req.data.to_uint());
  ack = sigs->read_ack();

  rsp.valid = ack;
  rsp.set_name("Acknowledge of data from master");
  rsp.print();

  if (!ack) {
    sc_core::wait(sigs->stop_e);
    rsp.stop_cond = true;
  }
  put_rsp(rsp, req, "Master acknoledge(or not) data, and maybe stop condition");
  return rsp;
}

i2c_rw i2c_driver::handle_write_to_slave(i2c_rw &req) {
  i2c_rw rsp;
  bool ack = false;
  rsp.data = sigs->read_data_byte();
  if (rsp.data == -1) {
    rsp.stop_cond = true;
    put_rsp(rsp, req, "Send stop condition to seq");
    return rsp;
  }
  UVM_INFO("Driver I2c", "Got data: " + std::to_string(rsp.data.to_uint()),
           uvm::UVM_LOW);
  rsp.kind_e = i2c::WRITE;
  rsp.addr = 0;
  rsp.valid = true;

  // Send bytes to sequence
  // Get acknowledge from sequence
  req = put_rsp_get_req(rsp, req, "Data byte from master");
  req.print();
  ack = req.valid;
  sigs->drive_ack(ack);

  return rsp;
}

bool i2c_driver::slave_drive_transaction() {
  i2c_rw req;
  i2c_rw rsp;
  decltype(req.kind_e) rw;
  seq_item_port->get_next_item(req); // dummy transaction
  req.print();

  if (!sigs->wait_start_condition())
    UVM_ERROR("I2C Slave",
              "No start condition, SCL low after SDA fell"); // terminate

  rsp.addr = sigs->read_addr();
  rw = rsp.kind_e = sigs->read_rw() ? i2c::READ : i2c::WRITE;
  rsp.valid = true;

  // Send Address and RW from Driver to sequence
  // Get Acknowledge from Sequence
  req = put_rsp_get_req(rsp, req, "I2C address and RW");
  req.print();

  sigs->drive_ack(req.valid);

  if (!req.valid) {
    UVM_ERROR("I2C Slave", "Address was not acknowledged by sequence");
    return false;
  }

  bool ack = true;
  bool got_stop = false;
  do {
    rsp = i2c_rw(); // clear
    if (rw == i2c::WRITE) {
      rsp = handle_write_to_slave(req);
      got_stop = rsp.stop_cond;
    } else if (rw == i2c::READ) {
      rsp = handle_read_from_slave(req);
      if (rsp.error)
        return false;
      got_stop = rsp.stop_cond;
    }

  } while (!got_stop); // while there is acknowledge on the bus, read data
  // } while (ack); // while there is acknowledge on the bus, read data
  // no more data

  return true;
}

void i2c_driver::run_phase(uvm::uvm_phase &phase) {
  sigs->scl.write("z");
  sigs->sda.write("z");

  if (mode == i2c::MASTER) {
  } else if (mode == i2c::SLAVE) {
    UVM_INFO("CONFIGURED AS Slave", "", uvm::UVM_MEDIUM);
    while (true) {
      slave_drive_transaction();
    }
  }
}

i2c_rw i2c_driver::read(const i2c_rw &req) {}

void i2c_driver::write(const i2c_rw &req) {}
