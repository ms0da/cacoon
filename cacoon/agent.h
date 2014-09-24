
#ifndef CACOON_AGENT_H
#define CACOON_AGENT_H

#include "comms.h"
#include "datastore.h"
#include <limits>
#include <string>

namespace cacoon {

    static const std::string STORE_LOCATION = "datastore.bin";
    
    namespace agent_id {
        using value_type = unsigned int;
        static const value_type DEFAULT_ID = std::numeric_limits<value_type>::min();
    }

    template<typename T>
    struct agent {
        using value_type = T;
        using id_type = agent_id::value_type;

        agent(id_type id = agent_id::DEFAULT_ID)
        :m_comms(T(id)), m_id(id), m_store(STORE_LOCATION) {
        }

        void set_id(id_type id) {
            m_id = id;
        }

        id_type get_id() const {
            return m_id;
        }

    private:
        

        id_type m_id;
        comms m_comms;
        datastore m_store;
    };
}

#endif
