
#ifndef CACOON_AGENT_H
#define CACOON_AGENT_H

namespace cacoon {
    struct agent {
        using id_type = unsigned int;

        agent();

        void set_id(id_type id);
        id_type get_id() const;

        static const id_type DEFAULT_ID;
    private:
        id_type m_id;
    };
}

#endif
