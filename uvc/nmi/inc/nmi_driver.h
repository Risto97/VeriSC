#ifndef NMI_DRIVER_H_
#define NMI_DRIVER_H_

#include <systemc>
#include <uvm>

#include "nmi_rw.h"
#include "nmi_if.h"
#include "nmi_modes.h"
#include "sysc/kernel/sc_event.h"

class nmi_driver : public uvm::uvm_driver<nmi_rw>
{
    public:
        UVM_COMPONENT_UTILS(nmi_driver);

        nmi_if* sigs;

        nmi::mode_t mode;

        nmi_driver(uvm::uvm_component_name name);

        virtual void build_phase(uvm::uvm_phase & phase);

        virtual void run_phase(uvm::uvm_phase & phase);

    private:
        void write(const nmi_rw req);
        nmi_rw read(const nmi_rw req);
        nmi_rw read_insn(const nmi_rw req);
};

#endif /* NMI_DRIVER_H_ */
