
#include "comms_file.h"
#include <fstream>
#include <sstream>

using cacoon::comms_file;
using std::string;
using std::ofstream;
using std::ifstream;
using std::ios_base;
using std::endl;
using std::getline;
using std::list;
using std::stringstream;

comms_file::comms_file(const comms_id& id)
:comms_impl(id) {
    stringstream ss;
    ss << id;
    m_read = ifstream(ss.str());
}

comms_file::~comms_file() {
    m_read.close();
}

comms_file::content_type comms_file::receive_stream() {
    m_read.clear();
    m_read.seekg(m_read.beg);
    content_type strs;
    for (string str; getline(m_read, str);) {
        strs.push_back(str);
    }
    return strs;
}

void comms_file::send_stream(const comms_id& dst, const string& str) {
    stringstream ss;
    ss << dst;
    ofstream out(ss.str(), ios_base::out | ios_base::app);
    if (out.is_open()) {
        out << str << endl;
        out.close();
    }
}
