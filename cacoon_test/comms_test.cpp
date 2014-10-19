
#include "../ext/catch/catch.hpp"
#include "../cacoon/message.h"
#include <iosfwd>

using std::string;
using std::stringstream;

using cacoon::comms::comms_id_type;
using cacoon::comms::message;
using cacoon::comms::exception::could_not_deserialize;

static const comms_id_type ID_SRC(42);
static const comms_id_type ID_DST(666);
static const string BODY_BASE("H3ll0");
static const string BODY_TO_ADD("world!!");

SCENARIO("get the source/destination ids", "[comms::message::header]") {
    message::header h(ID_SRC, ID_DST);
    REQUIRE(ID_SRC == h.get_src());
    REQUIRE(ID_DST == h.get_dst());
}

SCENARIO("header serialization/deserialization", "[comms::message::header]") {
    stringstream ss;
    GIVEN("the header is serialized to a stream") {
        message::header h(ID_SRC, ID_DST);
        h.serialize(ss);
        REQUIRE(!ss.str().empty());
        WHEN("the stream is deserialized") {
            message::header h_deserialize(ss);
            THEN("source and destination should be the same") {
                REQUIRE(h.get_src() == h_deserialize.get_src());
                REQUIRE(h.get_dst() == h_deserialize.get_dst());
            }
        }
    }
    GIVEN("the stream is empty") {
        WHEN("the header throws an exception") {
            REQUIRE_THROWS_AS(message::header h(ss), could_not_deserialize);
        }
    }
}

SCENARIO("adds and retrives data", "[comms::message::body]") {

    GIVEN("an empty message body") {
        message::body b;
        string body_content = b.get_data();
        REQUIRE(body_content.empty());

        WHEN("data is added") {
            b.add_data(BODY_TO_ADD);

            THEN("data can be retrieved") {
                body_content = b.get_data();
                REQUIRE(body_content == BODY_TO_ADD);
            }
        }
    }
    GIVEN("a non empty message body") {
        message::body b(BODY_BASE);
        string body_content = b.get_data();
        REQUIRE(!body_content.empty());
        REQUIRE(body_content == BODY_BASE);

        WHEN("data is added") {
            b.add_data(BODY_TO_ADD);

            THEN("data can be retrieved") {
                body_content = b.get_data();
                const string EXPECTED_CONTENT = BODY_BASE + BODY_TO_ADD;
                REQUIRE(EXPECTED_CONTENT == body_content);
            }
        }
    }    
}

SCENARIO("body serialization/deserialization", "[comms::message::body]") {
    stringstream ss;
    GIVEN("the stream is empty") {
        WHEN("the body throws an exception") {
            REQUIRE_THROWS_AS(message::body b(ss), could_not_deserialize);
        }
    }
    GIVEN("an empty body") {
        message::body b;
        WHEN("the body is serialized") {
            b.serialize(ss);
            THEN("the body throws an exception") {
                message::body b_deserialized(ss);
                REQUIRE(b.get_data() == b_deserialized.get_data());
            }
        }
    }
    GIVEN("a non empty body") {
        message::body b(BODY_BASE);
        WHEN("the body is serialized") {
            b.serialize(ss);
            THEN("the stream is deserialized") {
                message::body b_deserialized(ss);
                REQUIRE(b.get_data() == b_deserialized.get_data());
            }
        }
    }
}
