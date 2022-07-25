#ifndef NMI_SEQUENCE_HPP_
#define NMI_SEQUENCE_HPP_

#include <sstream>
#include <string>
#include <systemc>
#include <uvm>

#include <iostream>

#include <nmi_rw.h>
#include <nmi_modes.h>


class nmi_sequence : public uvm::uvm_sequence<nmi_rw, nmi_rw> {
public:

  std::vector<unsigned int> data;

  nmi_sequence(const std::string &name) : uvm::uvm_sequence<nmi_rw, nmi_rw>(name) {}

  UVM_OBJECT_PARAM_UTILS(nmi_sequence);

// Fill data field with this method
  void pre_body();
  void body();
  void post_body();
  void message(const nmi_rw &p);

};
#endif
