#ifndef APB_CONFIG_H_
#define APB_CONFIG_H_

#include <systemc>
#include <uvm>

class apb_config : public uvm::uvm_object
{
    public:
        UVM_OBJECT_UTILS(apb_config);

        apb_if* vif;

        apb_config(const std::string & name = "apb_config") {};
};

#endif /* APB_CONFIG_H */
