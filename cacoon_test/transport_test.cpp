
#include "../ext/catch/catch.hpp"

#include "../cacoon/transport_file.h"
#include <fstream>
#include <sstream>

using cacoon::transport::id_type;
using cacoon::transport::base;
using cacoon::transport::file;
using std::string;
using std::ifstream;
using std::ofstream;
using std::endl;
using std::string;
using std::stringstream;
using std::getline;
using std::move;

static const string content_line = "hello world !42";

static const id_type dst_read = 42;
static const id_type dst_write = 24;

string get_filename(const id_type& id) {
    stringstream ss;
    ss << id;
    return ss.str();
}

void write_line(const string location, const string content, bool truncate = true) {
    std::ios_base::openmode mode = std::ios_base::out;
    if (truncate) {
        mode |= std::ios_base::trunc;
    } else {
        mode |= std::ios_base::app;
    }
    ofstream os(location, mode);
    os << content;
    os.flush();
}

SCENARIO("transport needs a destination", "[transport_file]") {
    file tf(dst_read);
    base t(move(tf));
    REQUIRE(t.get_location() == dst_read);
}

SCENARIO("can write to a communication channel", "[transport_file]") {
    file tf(dst_read);
    base t(move(tf));

    WHEN("content is written") {
        t.send(dst_write, content_line);

        THEN("content is expected to disk") {
            ifstream in(get_filename(dst_write));
            string str;
            getline(in, str);
            REQUIRE(content_line == str);
        }
    }
}

SCENARIO("can read from a communication channel", "[transport_file]") {
    write_line(get_filename(dst_read), "");
    file tf(dst_read);
    base t(move(tf));
    REQUIRE(t.empty());

    WHEN("the channel is empty") {
        THEN("content is expected to be empty") {
            t.receive();
            REQUIRE(t.empty());
        }
    }
    WHEN("the channel has a single line to read") {
        write_line(get_filename(dst_read), content_line);

        THEN("content is not expected to be empty") {
            t.receive();
            REQUIRE(!t.empty());
            string content = t.get_string();
            REQUIRE(content == content_line);
        }
    }
    
    WHEN("the channel has multiple line to read") {
        const int line_count = 5;
        ofstream os(get_filename(dst_read));
        for (int i = 0; i < line_count; ++i) {
            os << i << content_line << std::endl;
        }

        THEN("content is not expected to be empty") {
            REQUIRE(t.empty());
            t.receive();
            REQUIRE(!t.empty());
            for (int i = 0; i < line_count; ++i) {
                stringstream ss;
                ss << i << content_line;
                string expected_content;
                getline(ss, expected_content);
                REQUIRE(!t.empty());
                string content = t.get_string();
                REQUIRE(content == expected_content);
            }
        }
    }
}
