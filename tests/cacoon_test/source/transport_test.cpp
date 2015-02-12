

#include <catch/catch.hpp>
#include "../include/test_utils.h"
#include <cacoon/include/transport_file.h>
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

SCENARIO("transport needs a destination", "[transport_file]") {
    file tf(dst_read);
    base t(move(tf));
    REQUIRE(t.get_id() == dst_read);
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
    const string filename_read = get_filename(dst_read);
    test_utils::empty_file(filename_read);
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
        test_utils::write_to_file(filename_read, content_line);

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
