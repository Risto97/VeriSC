#ifndef NMI_DRIVER_H_
#define NMI_DRIVER_H_

#include <systemc>
#include <uvm>

#include "nmi_rw.h"
#include "nmi_if.h"
#include "nmi_modes.h"
#include "sysc/kernel/sc_event.h"

namespace uvc::nmi{
    class driver : public uvm::uvm_driver<rw>
    {
        public:
            UVM_COMPONENT_UTILS(driver);

            intf* sigs;

            mode_t mode;

            driver(uvm::uvm_component_name name);

            virtual void build_phase(uvm::uvm_phase & phase);

            virtual void run_phase(uvm::uvm_phase & phase);

        private:
            void write(const rw req);
            rw read(const rw req);
            rw read_insn(const rw req);
    };
}

#endif /* NMI_DRIVER_H_ */
