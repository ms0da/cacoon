
#include "agent.h"
#include <limits>

using cacoon::agent;
using cacoon::comms_impl;
using std::numeric_limits;
using std::move;

const agent::id_type agent::DEFAULT_ID = numeric_limits<agent::id_type>::min();

agent::agent(comms_impl&& c)
:m_id(DEFAULT_ID), m_comms(move(c)) {
}

void agent::set_id(id_type id) {
    m_id = id;
}

agent::id_type agent::get_id() const {
    return m_id;
}
