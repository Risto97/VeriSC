#ifndef APB_DRIVER_H_
#define APB_DRIVER_H_

#include <systemc>
#include <uvm>

#include "apb_rw.h"
#include "apb_if.h"
#include "apb_modes.h"
#include <stdint.h>
// #include "apb_agent.h"

// namespace apb{
//     enum mode_t { MASTER = 0, SLAVE = 1 };
// }

namespace uvc::apb{
    class driver : public uvm::uvm_driver<rw>
    {
        public:
            UVM_COMPONENT_UTILS(driver);

            Intf* sigs;

            mode_t mode;
            uint32_t max_retry_count;


            driver(uvm::uvm_component_name name);

            virtual void build_phase(uvm::uvm_phase & phase);

            virtual void run_phase(uvm::uvm_phase & phase);

        private:
            rw read(const rw &req);
            void write(const rw &req);
    };
}

#endif /* APB_DRIVER_H_ */
