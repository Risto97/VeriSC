#include "nmi_sequence.h"
#include "nmi.h"
#include <cmath>
#include <scv.h>

  void nmi_sequence::pre_body() {
    // raise objection if started as a root nmi_sequence
    // if (this->starting_phase != NULL)
    //   this->starting_phase->raise_objection(this);

  }

  void nmi_sequence::body() {
      uvc::nmi::rw *req;
      uvc::nmi::rw *rsp;
    req = new uvc::nmi::rw();
    rsp = new uvc::nmi::rw();

    scv_smart_ptr<bool> write_tr("write_tr");
    scv_smart_ptr<uint32_t> addr_tr("addr_tr");
    addr_tr->keep_only(0, 1024);
    scv_smart_ptr<uint32_t> data_tr("data_tr");
    scv_smart_ptr<bool> valid_tr("valid_tr");
    // valid_tr->keep_only(true);

    scv_smart_ptr<uint32_t> strb_tr("strb_tr");
    strb_tr->keep_only(std::list<uint32_t>{ 0b0001, 0b0010, 0b0100, 0b1000, 0b1100, 0b0011, 0b1111});


    for(int i = 0; i < 1000000; i++){

        valid_tr->next();
        req->valid = valid_tr.read();

        write_tr->next();
        if(write_tr.read() == true)
            req->kind_e = uvc::nmi::rw_enum::WRITE;
        else
            req->kind_e = uvc::nmi::rw_enum::READ;

        addr_tr->next();
        req->addr = addr_tr->read();

        data_tr->next();
        req->data = data_tr->read();

        strb_tr->next();
        req->strb = strb_tr->read();

          message(*req);

          this->start_item(req);
          this->finish_item(req);
    }

    UVM_INFO(this->get_name(), "Finishing sequence", uvm::UVM_MEDIUM);
}

  void nmi_sequence::post_body() {
    // if (this->starting_phase != NULL)
    //   this->starting_phase->drop_objection(this);
        // this->starting_phase->drop_objection(this);
  }

  void nmi_sequence::message(const uvc::nmi::rw &p) {
    std::stringstream ss;
    ss << p.data;
    std::string msg = ss.str();
    // UVM_INFO(" Generated data:", msg, uvm::UVM_MEDIUM);
  }

