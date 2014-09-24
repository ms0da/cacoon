
#include "datastore.h"
#include <sstream>
#include <fstream>

using cacoon::datastore;
using std::make_pair;
using std::find;
using std::begin;
using std::endl;
using std::string;
using std::ios_base;
using std::getline;
using std::stringstream;
using std::string;
using std::ifstream;
using std::ofstream;

const string datastore::KEY_DELIM_SERIALIZE = "0";
const unsigned int datastore::KEY_DELIM_DESERIALIZE = 10;

datastore::datastore(const string& location)
:m_location(location) {
    from_disk();
}

datastore::~datastore() {
    to_disk();
}

bool datastore::read(const key_type&k, value_type** v) {
    const auto itt_begin = end(m_container);
    const auto itt = m_container.find(k);
    const bool is_read = itt_begin != itt;
    if (is_read) {
        *v = &itt->second;
    }
    return is_read;
}

bool datastore::write(const key_type& k, const value_type& v) {
    auto p = m_container.emplace(make_pair(k, v));
    return p.second;
}

void datastore::from_disk() {
    deserialize();
}

void datastore::deserialize() {
    ifstream in(m_location, ios_base::binary);
    in.seekg(ios_base::beg);
    for(string str; getline(in, str);) {
        stringstream ss(str);
        key_type k;
        value_type v;
        ss >> k;
        ss >> v;
        write(k / KEY_DELIM_DESERIALIZE, v);
    }
}

void datastore::to_disk() {
    serialize();
}

void datastore::serialize() {
    ofstream out(m_location, ios_base::binary | ios_base::trunc);
    for(const auto& elem : m_container) {
        out << elem.first << KEY_DELIM_SERIALIZE << elem.second << endl;
    }
}
