#ifndef UNIV_SUBSCRIBER_H_
#define UNIV_SUBSCRIBER_H_

#include <systemc>
#include <tlm.h>
#include <uvm>

template <class packet_type, class sb_t, void (sb_t::*F)(const packet_type& a)>
class univ_subscriber : public uvm::uvm_subscriber<packet_type> {
public:
  sb_t *sb;
  uvm::uvm_object *obj;

  UVM_COMPONENT_UTILS(univ_subscriber);

  univ_subscriber(uvm::uvm_component_name name)
    : uvm::uvm_subscriber<packet_type>(name){
      if(!uvm::uvm_config_db<uvm::uvm_object*>::get(this, "", "sb", obj)){
          UVM_FATAL("Universal subscriber", "Scoreboard not registered in database for universal subscriber");
      }

      sb = dynamic_cast<sb_t *>(obj);
  };

  virtual void write(const packet_type &p){
    (sb->*F)(p);
  }
};

#endif /* UNIV_SUBSCRIBER_H_ */
