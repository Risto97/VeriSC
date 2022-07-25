#include "scoreboard.h"
#include "apb_rw.h"
#include "nmi_rw.h"
#include "uvmsc/base/uvm_object_globals.h"


static sc_dt::sc_lv<32> strobe_to_mask(sc_dt::sc_lv<4> strb){
    sc_dt::sc_lv<32> mask = 0;
    for(int i = 0; i < strb.length(); i++){
        if(strb.bit(i) == 1){
            mask = mask | (0xff << i*8);

        }

    }
    return mask;
}

 void scoreboard::build_phase(uvm::uvm_phase &phase) {
    uvm::uvm_scoreboard::build_phase(phase);
    uvm::uvm_config_db<uvm_object *>::set(this, "apb_listener", "sb", this);

    apb_listener =
        univ_subscriber<apb_rw, scoreboard,
                        &scoreboard::apb_write>::type_id::create("apb_listener",
                                                                 this);
    assert(apb_listener);

    uvm::uvm_config_db<uvm_object *>::set(this, "nmi_listener", "sb", this);

    nmi_listener =
        univ_subscriber<nmi_rw, scoreboard,
                        &scoreboard::nmi_write>::type_id::create("nmi_listener",
                                                                 this);
    assert(nmi_listener);

  }

  void scoreboard::connect_phase(uvm::uvm_phase &phase) {
    apb_listener_imp.connect(apb_listener->analysis_export);
    nmi_listener_imp.connect(nmi_listener->analysis_export);
  }

  void scoreboard::run_phase(uvm::uvm_phase &phase){ check(); }

  void scoreboard::check(){
      apb_rw apb_p;
      nmi_rw nmi_p;

      uint32_t mem[1024];
      std::fill_n(mem, 1024, -1);

      while(true){
          sc_core::wait(apb_e & nmi_e);

          apb_p = apb_vec.back();
          nmi_p = nmi_vec.back();

          sb_cg.sample_cg(apb_p, nmi_p);
          // UVM_INFO("NMI", nmi_p.convert2string(), uvm::UVM_LOW);
          // UVM_INFO("APB", apb_p.convert2string(), uvm::UVM_LOW);


          // NMI2APB reference model
         
          if(nmi_p.kind_e == nmi::nmi_rw_enum::WRITE && apb_p.kind_e != apb_rw_enum::WRITE)
              UVM_ERROR("SCOREBOARD", "NMI and APB kind different");

          if(nmi_p.kind_e == nmi::nmi_rw_enum::WRITE && nmi_p.data != apb_p.data)
              UVM_ERROR("SCOREBOARD", "NMI and APB write data different");

          if(nmi_p.kind_e == nmi::nmi_rw_enum::WRITE && nmi_p.addr != apb_p.addr)
              UVM_ERROR("SCOREBOARD", "NMI and APB write addr different");

          if(nmi_p.kind_e == nmi::nmi_rw_enum::WRITE && nmi_p.strb != apb_p.strb)
              UVM_ERROR("SCOREBOARD", "NMI and APB strb different");

          if(nmi_p.kind_e == nmi::nmi_rw_enum::READ && nmi_p.addr != apb_p.addr)
              UVM_ERROR("SCOREBOARD", "NMI and APB read addr different");

          if(nmi_p.kind_e == nmi::nmi_rw_enum::READ && nmi_p.data != apb_p.data)
              UVM_ERROR("SCOREBOARD", "NMI and APB read data different");

          // APB_MEM slave reference model
          
          // WRITE TO MEMORY
          if(apb_p.kind_e == apb_rw_enum::WRITE){
              mem[apb_p.addr.to_uint()] = apb_p.data.to_uint() & strobe_to_mask(apb_p.strb).to_uint();
          }

          // APB_MEM slave checker
          // if(apb_p.kind_e == apb_rw_enum::READ && nmi_p.data != mem[nmi_p.addr.to_uint()])
          //     UVM_ERROR("SCOREBOARD", "APB_MEM_SLAVE wrong read");


      }
  }

  void scoreboard::apb_write(const apb_rw &p) {
    apb_vec.push_back(p);
    // UVM_INFO("PHASE_WRITE: ", p.str(), uvm::UVM_MEDIUM);
    apb_e.notify();
  }
  void scoreboard::nmi_write(const nmi_rw &p) {
    nmi_vec.push_back(p);
    // UVM_INFO("SIN_WRITE: ", p.str(), uvm::UVM_MEDIUM);
    nmi_e.notify();
  }

