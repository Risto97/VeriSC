#include "sct_assert.h"
#include <cstdint>
#include <ios>
#include <sstream>
#include <string>
#include <systemc>
#include <uvm>

#include "i2c_agent.h"
#include "i2c_if.h"
#include "i2c_monitor.h"
#include "i2c_rw.h"
#include "uvmsc/base/uvm_object_globals.h"

i2c_monitor::i2c_monitor(uvm::uvm_component_name name)
    : uvm::uvm_monitor(name), sigs(nullptr) {}

void i2c_monitor::build_phase(uvm::uvm_phase &phase) {
  uvm::uvm_monitor::build_phase(phase);

  i2c_agent *agent = dynamic_cast<i2c_agent *>(get_parent());
  if (agent) {
    sigs = agent->vif;
  } else {
    if (!uvm::uvm_config_db<i2c_if *>::get(this, "", "vif", sigs)) {
      UVM_FATAL("i2c/MON/NOVIF",
                "No virtual interface specified for this monitor instance");
    }
  }
}

void i2c_monitor::connect_phase(uvm::uvm_phase &phase) { static_assertions(); }

void i2c_monitor::static_assertions() {}

void i2c_monitor::dynamic_assertions() {
  // while (1) {
  //   sigs->wait_posedge_rv(sigs->scl);
  // }
}

i2c_rw i2c_monitor::monitor_transaction() {
  i2c_rw rcv;

  if (!sigs->wait_start_condition())
    UVM_ERROR("I2C", "No start condition, SCL low after SDA fell"); // terminate

  rcv.addr = sigs->read_addr();
  rcv.kind_e =
      sigs->read_rw() ? i2c::i2c_rw_enum::READ : i2c::i2c_rw_enum::WRITE;

  bool ack = sigs->read_ack();
  if(!ack)
      UVM_ERROR("I2C", "Slave has not acknowledged Address");
  
  do {
    rcv.data = sigs->read_data_byte();

    ack = sigs->read_ack();
  } while (ack); // while there is acknowledge on the bus, read data
  // no more data
  if (!ack) {
    if (rcv.kind_e == i2c::i2c_rw_enum::WRITE) {
      UVM_ERROR("I2C", "Slave has not acknowledged WRITE");
      rcv.valid = false;
    } else {
      rcv.valid = true;
    }

    sigs->wait_stop_condition();
    return rcv; // finish
  }

  return rcv;
}

void i2c_monitor::run_monitor() {
  i2c_rw *tr;
  tr = i2c_rw::type_id::create("tr", this);

  UVM_INFO("Running Monitor", "Monitor", uvm::UVM_LOW);

  while (1) {
    uint32_t addr = 0;
    uint32_t data = 0;
    uint32_t rw = 0;   // 1 read, 0 write
    uint32_t nack = 1; // 1 not acknowledged, 0 acknowledged

    i2c_rw rec = monitor_transaction();
    std::stringstream ss;
    ss << "Addr: 0x" << std::hex << rec.addr << " Data: "
       << "0x" << std::hex << rec.data << " RW " << rw << "\n";
    UVM_INFO("I2C", ss.str(), uvm::UVM_LOW);
  }
}

void i2c_monitor::run_phase(uvm::uvm_phase &phase) {
  SC_FORK
  // sc_core::sc_spawn(sc_core::sc_bind(&i2c_monitor::run_monitor, this)),
      sc_core::sc_spawn(
          sc_core::sc_bind(&i2c_monitor::dynamic_assertions, this)),
      SC_JOIN
}
