
#include <cacoon/include/transport.h>
#include <string>
#include <algorithm>

using cacoon::transport::id_type;
using cacoon::transport::base;
using cacoon::transport::impl;
using std::string;
using std::begin;
using std::end;
using std::move;
using std::make_unique;

base::base(impl&& impl) {
    m_impl = &impl;
}

base::~base() {
}

void base::send(const id_type& dst, const string& str) {
    m_impl->send_stream(dst, str);
}

void base::receive() {
    auto new_content = m_impl->receive_stream();
    m_content.insert(end(m_content), begin(new_content), end(new_content));
}

const string base::get_string() {
    string line(m_content.front());
    m_content.pop_front();
    return line;
}

bool base::empty() const throw() {
    return m_content.empty();
}

const id_type& base::get_id() const throw() {
    return m_impl->get_id();
}

// IMPL specific
impl::impl(const id_type& id)
:m_id(id) {
}

const id_type& impl::get_id() const throw() {
    return m_id;
}
