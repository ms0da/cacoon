
#ifndef CACOON_AGENT_H
#define CACOON_AGENT_H

#include "comms.h"

namespace cacoon {
    struct agent {
        using id_type = unsigned int;

        agent(comms_impl&& c);

        void set_id(id_type id);
        id_type get_id() const;

        static const id_type DEFAULT_ID;

    private:
        id_type m_id;
        comms m_comms;
    };
}

#endif
