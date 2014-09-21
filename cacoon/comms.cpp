
#include "comms.h"
#include <string>
#include <algorithm>

using cacoon::comms;
using cacoon::comms_impl;
using std::string;
using std::begin;
using std::end;
using std::move;
using std::make_unique;

comms::comms(comms_impl&& impl) {
    //m_impl = make_unique<comms_impl>(impl);
    m_impl = &impl;
}

comms::~comms() {
}

void comms::send(const comms_id& dst, const string& str) {
    m_impl->send_stream(dst, str);
}

void comms::receive() {
    auto new_content = m_impl->receive_stream();
    m_content.insert(end(m_content), begin(new_content), end(new_content));
}

const string comms::get_string() {
    string line(m_content.front());
    m_content.pop_front();
    return move(line);
}

bool comms::empty() const throw() {
    return m_content.empty();
}

const comms::comms_id& comms::get_location() const throw() {
    return m_impl->get_location();
}

// IMPL specific
comms_impl::comms_impl(const comms_id& id)
:m_id(id) {
}

const comms_impl::comms_id& comms_impl::get_location() const throw() {
    return m_id;
}
