
#include "transport_file.h"
#include <fstream>
#include <sstream>

using cacoon::transport_file;
using std::string;
using std::ofstream;
using std::ifstream;
using std::ios_base;
using std::endl;
using std::getline;
using std::list;
using std::stringstream;

transport_file::transport_file(const transport_id& id)
:transport_impl(id) {
    stringstream ss;
    ss << id;
    m_read = ifstream(ss.str());
}

transport_file::~transport_file() {
    m_read.close();
}

transport_file::content_type transport_file::receive_stream() {
    m_read.clear();
    m_read.seekg(m_read.beg);
    content_type strs;
    for (string str; getline(m_read, str);) {
        strs.push_back(str);
    }
    return strs;
}

void transport_file::send_stream(const transport_id& dst, const string& str) {
    stringstream ss;
    ss << dst;
    ofstream out(ss.str(), ios_base::out | ios_base::app);
    if (out.is_open()) {
        out << str << endl;
        out.close();
    }
}
