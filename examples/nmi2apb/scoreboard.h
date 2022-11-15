#ifndef SCOREBOARD_H_
#define SCOREBOARD_H_

#include <cstdint>
#include <fc4sc.hpp>
#include <sys/types.h>
#include <systemc>
#include <uvm>
#include <vector>

#include "apb.h"
#include "nmi.h"
#include "univ_subscriber.h"

class sb_coverage : public fc4sc::covergroup {
public:
  uvc::apb::rw apb_p;
  uvc::nmi::rw nmi_p;

  CG_CONS(sb_coverage) {}

  void sample_cg(uvc::apb::rw apb_i, uvc::nmi::rw nmi_i) {

    apb_p = apb_i;
    nmi_p = nmi_i;

    covergroup::sample();
  }

  std::vector<uint32_t> get_bins_from_range(uint32_t range) {
    std::vector<uint32_t> bins_vec;
    for (int i = 0; i < range; i++) {
      bins_vec.push_back(i);
    }
    return bins_vec;
  }

  COVERPOINT(uint32_t, data_wr_strb, apb_p.strb.to_uint(),
             apb_p.kind_e == uvc::apb::WRITE){
      bin<uint32_t>("write_0001", 0b0001), bin<uint32_t>("write_0010", 0b0011),
      bin<uint32_t>("write_0100", 0b0100), bin<uint32_t>("write_1000", 0b1000),
      bin<uint32_t>("write_0011", 0b0011), bin<uint32_t>("write_1100", 0b1100),
#ifndef SCOREBOARD_H_
#define SCOREBOARD_H_

#include <cstdint>
#include <fc4sc.hpp>
#include <systemc>
#include <uvm>
#include <vector>

#include "apb.h"
#include "nmi.h"
#include "univ_subscriber.h"

class sb_coverage : public fc4sc::covergroup {
public:
    uvc::apb::rw apb_p;
    uvc::nmi::rw nmi_p;

  CG_CONS(sb_coverage) {}

  void sample_cg(uvc::apb::rw apb_i, uvc::nmi::rw nmi_i) {

    apb_p = apb_i;
    nmi_p = nmi_i;

    covergroup::sample();
  }

  std::vector<uint32_t> get_bins_from_range(uint32_t range) {
    std::vector<uint32_t> bins_vec;
    for (int i = 0; i < range; i++) {
      bins_vec.push_back(i);
    }
    return bins_vec;
  }

  COVERPOINT(uint32_t, data_wr_strb, apb_p.strb.to_uint(), apb_p.kind_e == uvc::apb::WRITE){
            bin<uint32_t>("write_0001", 0b0001),
            bin<uint32_t>("write_0010", 0b0011),
            bin<uint32_t>("write_0100", 0b0100),
            bin<uint32_t>("write_1000", 0b1000),
            bin<uint32_t>("write_0011", 0b0011),
            bin<uint32_t>("write_1100", 0b1100),
            bin<uint32_t>("write_1111", 0b1111)
  };

  COVERPOINT(uint32_t, kind_cvp, (uint32_t) apb_p.kind_e){
            bin<uint32_t>("READ",  uvc::apb::rw_enum::READ),
            bin<uint32_t>("WRITE", uvc::apb::rw_enum::WRITE)
  };

  COVERPOINT(uint32_t, data_wr_cvp, apb_p.data.to_uint(), apb_p.kind_e == uvc::apb::WRITE){
            bin<uint32_t>("data", interval(0, INT_MIN))};

  COVERPOINT(uint32_t, w_addr_split_cvp, apb_p.addr.to_uint(), apb_p.valid){
            bin_array<uint32_t>("memory", get_bins_from_range(1024)),
            ignore_bin<uint32_t>("Rest", interval(1024, INT_MIN))};


};

class scoreboard : public uvm::uvm_scoreboard {
public:
  std::vector<uvc::apb::rw> apb_vec;
  std::vector<uvc::nmi::rw> nmi_vec;

  sc_core::sc_event apb_e;
  sc_core::sc_event nmi_e;

  uvm::uvm_analysis_export<uvc::apb::rw> apb_listener_imp;
  uvm::uvm_analysis_export<uvc::nmi::rw> nmi_listener_imp;

  void check();

  void apb_write(const uvc::apb::rw &p);
  void nmi_write(const uvc::nmi::rw &p);

  univ_subscriber<uvc::apb::rw, scoreboard, &scoreboard::apb_write> *apb_listener;
  univ_subscriber<uvc::nmi::rw, scoreboard, &scoreboard::nmi_write> *nmi_listener;

  sb_coverage sb_cg;

  scoreboard(uvm::uvm_component_name name) : uvm::uvm_scoreboard(name) {}
  UVM_COMPONENT_UTILS(scoreboard);

  void build_phase(uvm::uvm_phase &phase);

  void connect_phase(uvm::uvm_phase &phase);

  void run_phase(uvm::uvm_phase &phase);
};

#endif
      bin<uint32_t>("write_1111", 0b1111)};

  COVERPOINT(uint32_t, kind_cvp, (uint32_t)apb_p.kind_e){
      bin<uint32_t>("READ", uvc::apb::rw_enum::READ),
      bin<uint32_t>("WRITE", uvc::apb::rw_enum::WRITE)};

  COVERPOINT(uint32_t, data_wr_cvp, apb_p.data.to_uint(),
             apb_p.kind_e ==
                 uvc::apb::WRITE){bin<uint32_t>("data", interval(0, INT_MIN))};

  COVERPOINT(uint32_t, w_addr_split_cvp, apb_p.addr.to_uint(), apb_p.valid){
      bin_array<uint32_t>("memory", get_bins_from_range(1024)),
      ignore_bin<uint32_t>("Rest", interval(1024, INT_MIN))};
};

class scoreboard : public uvm::uvm_scoreboard {
public:
  std::vector<uvc::apb::rw> apb_vec;
  std::vector<uvc::nmi::rw> nmi_vec;

  sc_core::sc_event apb_e;
  sc_core::sc_event nmi_e;

  uvm::uvm_analysis_export<uvc::apb::rw> apb_listener_imp;
  uvm::uvm_analysis_export<uvc::nmi::rw> nmi_listener_imp;

  void check();

  void apb_write(const uvc::apb::rw &p);
  void nmi_write(const uvc::nmi::rw &p);

  uvc::univ_subscriber<uvc::apb::rw, scoreboard, &scoreboard::apb_write> *apb_listener;
  uvc::univ_subscriber<uvc::nmi::rw, scoreboard, &scoreboard::nmi_write> *nmi_listener;

  sb_coverage sb_cg;

  scoreboard(uvm::uvm_component_name name) : uvm::uvm_scoreboard(name) {}
  UVM_COMPONENT_UTILS(scoreboard);

  void build_phase(uvm::uvm_phase &phase);

  void connect_phase(uvm::uvm_phase &phase);

  void run_phase(uvm::uvm_phase &phase);
};

#endif
