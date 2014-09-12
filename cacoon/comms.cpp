
#include "comms.h"
#include <string>

using cacoon::comms;
using std::string;
using std::begin;
using std::end;
using std::move;

comms::comms(const comms_id& location)
:m_location(location) {
}

comms::~comms() {
}

void comms::write(const comms_id& dst, const string& str) {
    write_stream(dst, str);
}

void comms::update() {
    auto new_content = update_stream();
    m_content.insert(end(m_content), begin(new_content), end(new_content));
}

const string comms::read() {
    string line(m_content.front());
    m_content.pop_front();
    return move(line);
}

bool comms::empty() const throw() {
    return m_content.empty();
}

const string& comms::get_location() const throw() {
    return m_location;
}
