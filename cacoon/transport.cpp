
#include "transport.h"
#include <string>
#include <algorithm>

using cacoon::transport;
using cacoon::transport_impl;
using std::string;
using std::begin;
using std::end;
using std::move;
using std::make_unique;

transport::transport(transport_impl&& impl) {
    //m_impl = make_unique<comms_impl>(impl);
    m_impl = &impl;
}

transport::~transport() {
}

void transport::send(const transport_id& dst, const string& str) {
    m_impl->send_stream(dst, str);
}

void transport::receive() {
    auto new_content = m_impl->receive_stream();
    m_content.insert(end(m_content), begin(new_content), end(new_content));
}

const string transport::get_string() {
    string line(m_content.front());
    m_content.pop_front();
    return move(line);
}

bool transport::empty() const throw() {
    return m_content.empty();
}

const transport::transport_id& transport::get_location() const throw() {
    return m_impl->get_location();
}

// IMPL specific
transport_impl::transport_impl(const transport_id& id)
:m_id(id) {
}

const transport_impl::transport_id& transport_impl::get_location() const throw() {
    return m_id;
}
