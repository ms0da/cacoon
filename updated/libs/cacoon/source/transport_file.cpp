
#include <cacoon/include/transport_file.h>
#include <fstream>
#include <sstream>

using cacoon::transport::id_type;
using cacoon::transport::file;
using std::string;
using std::ofstream;
using std::ifstream;
using std::ios_base;
using std::endl;
using std::getline;
using std::list;
using std::stringstream;

file::file(const id_type& id)
:impl(id) {
    stringstream ss;
    ss << id;
    m_read = ifstream(ss.str());
}

file::~file() {
    m_read.close();
}

cacoon::transport::content_type file::receive_stream() {
    m_read.clear();
    m_read.seekg(m_read.beg);
    content_type strs;
    for (string str; getline(m_read, str);) {
        strs.push_back(str);
    }
    return strs;
}

void file::send_stream(const id_type& dst, const string& str) {
    stringstream ss;
    ss << dst;
    ofstream out(ss.str(), ios_base::out | ios_base::app);
    if (out.is_open()) {
        out << str << endl;
        out.close();
    }
}
