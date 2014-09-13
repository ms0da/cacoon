
#include "comms_file.h"
#include <fstream>

using cacoon::comms_file;
using std::string;
using std::ofstream;
using std::ifstream;
using std::ios_base;
using std::endl;
using std::getline;
using std::copy;
using std::list;

comms_file::comms_file(const comms_id& location)
:comms_impl(location), m_read(location) {
}

comms_file::~comms_file() {
    m_read.close();
}

comms_file::content_type comms_file::update_stream() {
    m_read.clear();
    m_read.seekg(m_read.beg);
    content_type strs;
    for (string str; getline(m_read, str);) {
        strs.push_back(str);
    }
    return strs;
}

void comms_file::write_stream(const comms_id& dst, const string& str) {
    ofstream out(dst, ios_base::out | ios_base::app);
    if (out.is_open()) {
        out << str << endl;
        out.close();
    }
}
