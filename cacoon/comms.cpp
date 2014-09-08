
#include "comms.h"
#include <fstream>
#include <sstream>

using cacoon::comms;
using std::string;
using std::ofstream;
using std::ifstream;
using std::ios_base;
using std::endl;
using std::stringstream;
using std::getline;
using std::copy;
using std::list;

#include <iostream>
#include <cassert>

using is_itt = std::istream_iterator<string>;

comms::comms(const comms_id location)
:m_location(location),
m_read(ifstream(location)){
    update();
}

list<string> comms::update_stream() {
    m_read.clear();
    m_read.seekg(m_read.beg);    
    list<string> strs;
    for (string str; getline(m_read, str);) {
        strs.push_back(str);
    }
    return strs;
}

const string& comms::get_location() const throw() {
    return m_location;
}

void comms::write(const string& dst, const string& content) {
    ofstream out(dst, ios_base::out | ios_base::app);
    if (out.is_open()) {
        out << content << endl;
        out.close();
    }
}

void comms::update() {
    auto content = update_stream();
    m_content.insert(std::end(m_content), std::begin(content), std::end(content));
}

string comms::read() {
    string line(m_content.front());
    m_content.pop_front();
    return line;
}

bool comms::empty() const throw() {
    return m_content.empty();
}
