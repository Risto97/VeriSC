#ifndef __ENV_H__
#define __ENV_H__

#include <systemc>
#include <uvm>
#include <apb.h>

class env : public uvm::uvm_env
{
    public:
        UVM_COMPONENT_UTILS(env);
        uvc::apb::agent *ag;
        uvc::apb::Intf *vif;
        sc_core::sc_clock clk;

    env(uvm::uvm_component_name name)
        : uvm::uvm_env(name), clk("clk", 10, sc_core::SC_NS){

        }

    virtual void build_phase(uvm::uvm_phase& phase)
    {
        uvm::uvm_env::build_phase(phase);

      vif = new uvc::apb::Intf("apb_slave", &clk);
      ag = new uvc::apb::agent("apb_agent_slave");
      uvm::uvm_config_db<int>::set(this, "apb_agent_slave", "is_active",
                                   !uvm::UVM_ACTIVE);
      uvm::uvm_config_db<uvc::apb::mode_t>::set(this, "apb_agent_slave", "mode",
                                                uvc::apb::SLAVE);
      uvm::uvm_config_db<uvc::apb::Intf *>::set(this, "apb_agent_slave", "vif",
                                            vif);


    }

    void connect_phase(uvm::uvm_phase& phase){

    }


};

#endif
