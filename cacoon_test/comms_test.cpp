
#include "../ext/catch/catch.hpp"

#include "../cacoon/comms_file.h"
#include <fstream>
#include <sstream>

using cacoon::comms_file;
using std::string;
using std::ifstream;
using std::ofstream;
using std::endl;
using std::string;
using std::stringstream;
using std::getline;


static const string base_dir = "C:/code/cacoon/Debug/";
static const string dst_read = base_dir + "comms_test_read";
static const string dst_write = base_dir + "comms_test_write";
static const string content_line = "hello world !42";

void write_line(const string location, const string content, bool truncate = true) {
    std::ios_base::openmode mode = std::ios_base::out;
    if (truncate) {
        mode |= std::ios_base::trunc;
    }
    else {
        mode |= std::ios_base::app;
    }
    ofstream os(location, mode);
    os << content;
    os.flush();
}

SCENARIO("comms needs a destination", "[comms_file]") {
    comms_file c(dst_read);
    REQUIRE(c.get_location() == dst_read);
}

SCENARIO("can write to a communication channel", "[comms_file]") {
    comms_file c(dst_read);

    WHEN("content is written") {
        c.write(dst_write, content_line);

        THEN("content is expected to disk") {
            ifstream in(dst_write);
            string str;
            getline(in, str);
            REQUIRE(content_line == str);
        }
    }
}

SCENARIO("can read from a communication channel", "[comms_file]") {
    write_line(dst_read, "");
    comms_file c(dst_read);
    REQUIRE(c.empty());

    WHEN("the channel is empty") {
        THEN("content is expected to be empty") {
            c.update();
            REQUIRE(c.empty());
        }
    }
    WHEN("the channel has a single line to read") {
        write_line(dst_read, content_line);

        THEN("content is not expected to be empty") {
            c.update();
            REQUIRE(!c.empty());
            string content = c.read();
            REQUIRE(content == content_line);
        }
    }
    
    WHEN("the channel has multiple line to read") {
        const int line_count = 5;
        ofstream os(dst_read);
        for (int i = 0; i < line_count; ++i) {
            os << i << content_line << std::endl;
        }

        THEN("content is not expected to be empty") {
            REQUIRE(c.empty());
            c.update();
            REQUIRE(!c.empty());
            for (int i = 0; i < line_count; ++i) {
                stringstream ss;
                ss << i << content_line;
                string expected_content;
                getline(ss, expected_content);
                REQUIRE(!c.empty());
                string content = c.read();
                REQUIRE(content == expected_content);
            }
        }
    }
}
