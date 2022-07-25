#ifndef APB_SEQUENCE_HPP_
#define APB_SEQUENCE_HPP_

#include <sstream>
#include <string>
#include <systemc>
#include <uvm>

#include <iostream>

#include <apb_rw.h>


class apb_sequence : public uvm::uvm_sequence<apb_rw, apb_rw> {
public:

  std::vector<unsigned int> data;
    uint32_t mem[1024];

  apb_sequence(const std::string &name) : uvm::uvm_sequence<apb_rw, apb_rw>(name) {}

  UVM_OBJECT_PARAM_UTILS(apb_sequence);

// Fill data field with this method

  void pre_body();

  void body();

  void post_body();

  void message(const apb_rw &p);

};
#endif
