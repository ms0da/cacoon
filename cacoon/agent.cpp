
#include "agent.h"
using cacoon::agent;

#include <limits>
using std::numeric_limits;

const agent::id_type agent::DEFAULT_ID = numeric_limits<agent::id_type>::min();

agent::agent()
:m_id(DEFAULT_ID) {
}

void agent::set_id(id_type id) {
    m_id = id;
}

agent::id_type agent::get_id() const {
    return m_id;
}
