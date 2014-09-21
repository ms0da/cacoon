
#include "../ext/catch/catch.hpp"

#include "../cacoon/comms_file.h"
#include <fstream>
#include <sstream>

using cacoon::comms;
using cacoon::comms_file;
using cacoon::comms_types::comms_id;
using std::string;
using std::ifstream;
using std::ofstream;
using std::endl;
using std::string;
using std::stringstream;
using std::getline;
using std::move;

static const string content_line = "hello world !42";

static const comms_id dst_read = 42;
static const comms_id dst_write = 24;

string get_filename(const comms_id id) {
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

SCENARIO("comms needs a destination", "[comms_file]") {
    comms_file cf(dst_read);
    comms c(move(cf));
    REQUIRE(c.get_location() == dst_read);
}

SCENARIO("can write to a communication channel", "[comms_file]") {
    comms_file cf(dst_read);
    comms c(move(cf));

    WHEN("content is written") {
        c.send(dst_write, content_line);

        THEN("content is expected to disk") {
            ifstream in(get_filename(dst_write));
            string str;
            getline(in, str);
            REQUIRE(content_line == str);
        }
    }
}

SCENARIO("can read from a communication channel", "[comms_file]") {
    write_line(get_filename(dst_read), "");
    comms_file cf(dst_read);
    comms c(move(cf));
    REQUIRE(c.empty());

    WHEN("the channel is empty") {
        THEN("content is expected to be empty") {
            c.receive();
            REQUIRE(c.empty());
        }
    }
    WHEN("the channel has a single line to read") {
        write_line(get_filename(dst_read), content_line);

        THEN("content is not expected to be empty") {
            c.receive();
            REQUIRE(!c.empty());
            string content = c.get_string();
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
            REQUIRE(c.empty());
            c.receive();
            REQUIRE(!c.empty());
            for (int i = 0; i < line_count; ++i) {
                stringstream ss;
                ss << i << content_line;
                string expected_content;
                getline(ss, expected_content);
                REQUIRE(!c.empty());
                string content = c.get_string();
                REQUIRE(content == expected_content);
            }
        }
    }
}
