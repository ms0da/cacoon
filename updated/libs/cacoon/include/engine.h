
#ifndef CACOON_COMMS_H
#define CACOON_COMMS_H

#include "module.h"
#include "transport.h"
#include <iosfwd>

namespace cacoon {
    namespace comms {

        struct engine : public cacoon::common::module {
            using transport_type = cacoon::transport::base;

            engine(transport_type& transport)
            :module(std::chrono::milliseconds(500)), m_transport(transport) {
            }

            virtual ~engine() {
            }
        private:
            virtual void loop() override {
                m_transport.receive();
                if(!m_transport.empty()) {
                    const auto trans_str = m_transport.get_string();
                    std::cout << "TRANSPORT STRING:" << trans_str;


                }
                std::cout << "e";
            }

            transport_type& m_transport;
        };

    }
}

#endif
