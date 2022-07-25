#include "apb_sequence.h"
#include <cmath>

void apb_sequence::pre_body() {
  // raise objection if started as a root apb_sequence
  // if (this->starting_phase != NULL)
  //   this->starting_phase->raise_objection(this);

}

void apb_sequence::body() {
  apb_rw *req;
  apb_rw *rsp;
  req = new apb_rw();
  rsp = new apb_rw();

  for (auto iter0 = data.begin(); iter0 != data.end(); ++iter0) {

    // req->data = *iter0;
    // message(*req);

    this->get_response(rsp);
    message(*rsp);

    if (rsp->strb.to_uint() > 0) {
      mem[rsp->addr.to_uint()] = rsp->data.to_uint();
    } else {
      req->data = mem[rsp->addr.to_uint()];
    }

    this->start_item(req);
    this->finish_item(req);
  }
  UVM_INFO(this->get_name(), "Finishing sequence", uvm::UVM_MEDIUM);
}

void apb_sequence::post_body() {
  // if (this->starting_phase != NULL)
  //   this->starting_phase->drop_objection(this);
  // this->starting_phase->drop_objection(this);
}

void apb_sequence::message(const apb_rw &p) {
  std::stringstream ss;
  ss << p.data;
  std::string msg = ss.str();
  UVM_INFO(" Generated data:", msg, uvm::UVM_MEDIUM);
}
