#ifndef NMI_SEQUENCE_H_
#define NMI_SEQUENCE_H_

#include <sstream>
#include <string>
#include <systemc>
#include <uvm>

#include <iostream>

#include <nmi.h>


class nmi_sequence : public uvm::uvm_sequence<uvc::nmi::rw, uvc::nmi::rw> {
public:

  std::vector<unsigned int> data;

  nmi_sequence(const std::string &name) : uvm::uvm_sequence<uvc::nmi::rw, uvc::nmi::rw>(name) {}

  UVM_OBJECT_PARAM_UTILS(nmi_sequence);

// Fill data field with this method
  void pre_body();
  void body();
  void post_body();
  void message(const uvc::nmi::rw &p);

};
#endif
