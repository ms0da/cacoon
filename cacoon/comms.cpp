
#include "comms.h"

using std::ostream;
using std::istream;
using std::unique_ptr;
using std::make_unique;
using std::string;
using std::move;

// SERIALIZABLE
using cacoon::comms::serializable;
using cacoon::comms::exception::could_not_deserialize;

void serializable::throw_could_not_deserialize() {
    throw could_not_deserialize("Could not deserialize object");
}

// HEADER
using cacoon::comms::comms_id_type;
using cacoon::comms::message::header;

header::header(istream& is) {
    deserialize(is);
}

header::header(const comms_id_type& src, const comms_id_type& dst)
:m_src(src), m_dst(dst) {
}

header::~header() {
}

const comms_id_type& header::get_src() const throw() {
    return m_src;
}

const comms_id_type& header::get_dst() const throw() {
    return m_dst;
}

void header::serialize(ostream& os) {
    os << m_src << " " << m_dst;
}

void header::deserialize(std::istream& is) {
    comms_id_type src, dst;
    unique_ptr<serializable> ptr(nullptr);
    if(is >> src && is >> dst) {
        this->m_src = src;
        this->m_dst = dst;
    } else {
        throw_could_not_deserialize();
    }
}

// BODY
using cacoon::comms::message::body;

body::body(istream& is) {
    deserialize(is);
}

body::body(const string& data) 
:m_data(data) {
}

body::~body() {
}

void body::add_data(const string& data) {
    m_data.append(data);
}

std::string body::get_data() const throw() {
    return m_data;
}

void body::serialize(ostream& os) {
    os << m_data;
}

void body::deserialize(std::istream& is) {
    string str;
    if(is >> str) {
        m_data = move(str);
    } else {
        throw_could_not_deserialize();
    }
}
