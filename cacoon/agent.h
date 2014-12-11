
#ifndef CACOON_AGENT_H
#define CACOON_AGENT_H

#include "module.h"
#include "transport.h"
#include "datastore.h"
#include <limits>
#include <string>
#include <iostream>

namespace cacoon {

    static const std::string STORE_LOCATION = "datastore.bin";
    
    namespace agent_id {
        using value_type = unsigned int;
        static const value_type DEFAULT_ID = std::numeric_limits<value_type>::min();
    }

    template<typename transport_type>
    struct agent : public common::module {
        using id_type = agent_id::value_type;

        agent(id_type id = agent_id::DEFAULT_ID)
        :module(std::chrono::milliseconds(0)), m_transport(transport_type(id)), m_id(id), m_store(STORE_LOCATION) {
        }

        virtual ~agent() {
        }

        void set_id(id_type id) {
            m_id = id;
        }

        id_type get_id() const {
            return m_id;
        }
    private:
        virtual void loop() override {
            std::cout << ".";
        }

        id_type m_id;
        transport::base m_transport;
        datastore m_store;
    };
}

#endif
