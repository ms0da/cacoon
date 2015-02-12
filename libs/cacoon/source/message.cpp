
#include <cacoon/include/message.h>
#include <sstream>

using cacoon::comms::comms_id_type;
using cacoon::comms::message;
using cacoon::comms::serializable;
using cacoon::comms::exception::could_not_deserialize;

using std::ostream;
using std::istream;
using std::unique_ptr;
using std::make_unique;
using std::string;
using std::move;
using std::stringstream;
using std::make_unique;

// HEADER
message::header::header(istream& is) {
    if(!deserialize(is)) {
        throw could_not_deserialize("Could not deserialize message::header");
    }
}

message::header::header(const comms_id_type& src, const comms_id_type& dst)
:m_src(src), m_dst(dst) {
}

message::header::~header() {
}

const comms_id_type& message::header::get_src() const throw() {
    return m_src;
}

const comms_id_type& message::header::get_dst() const throw() {
    return m_dst;
}

void message::header::serialize(ostream& os) const {
    os << m_src << " " << m_dst;
}

bool message::header::deserialize(std::istream& is) {
    comms_id_type src = 0;
    comms_id_type dst = 0;
    bool is_read = is >> src && is >> dst;
    if(is_read) {
        this->m_src = src;
        this->m_dst = dst;
    }
    return is_read;
}

// BODY
const char message::body::DELIM_START = '~';

message::body::body() {
}

message::body::body(istream& is) {
    if(!deserialize(is)) {
        throw could_not_deserialize("Could not deserialize message::body");
    }
}

message::body::~body() {
}

bool message::body::is_empty() const {
    return m_list.empty();
}

void message::body::add(const serializable&& obj) {
    m_list.emplace_back(make_unique<serializable>(obj));
}

void message::body::serialize(ostream& os) const {
    os << DELIM_START;
    for(const auto &obj : m_list) {
        //obj->serialize(os);
        (obj);
    }
}

// TODO complete deserialization
bool message::body::deserialize(istream& is) {
    bool is_read = is.good();
    if(is_read) {
        auto c = is.peek();
        is_read = DELIM_START == c;
        if(is_read) {
            c = is.get();

            //m_list.emplace_back(

        }
    }
    //string str;
    //is >> str;
    //bool is_read = !(is.fail() || is.bad());
    //bool is_delim_present = str.length() > 0 ? DELIM_START == str.at(0) : false;
    //if(is_read && is_delim_present) {
    //    m_data = move(str.substr(1, str.length()));
    //}
    return is_read;
}

// MESSAGE
message::message(istream& is)
:m_header(is), m_body(is) {
}

message::message(const comms_id_type& src, const comms_id_type& dst)
:m_header(src, dst) {
}

const comms_id_type& message::get_src() const throw() {
    return m_header.get_src();
}

const comms_id_type& message::get_dst() const throw() {
    return m_header.get_dst();
}

void message::append(const serializable&& obj) {
    m_body.add(move(obj));
}

void message::serialize_type(ostream& os) const {
    m_header.serialize(os);
    m_body.serialize(os);
}
