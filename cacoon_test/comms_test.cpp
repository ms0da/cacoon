
#include "../ext/catch/catch.hpp"
#include "../cacoon/message.h"
#include <iosfwd>

using std::string;
using std::stringstream;

using cacoon::comms::comms_id_type;
using cacoon::comms::message::header;
using cacoon::comms::exception::could_not_deserialize;

static const comms_id_type ID_SRC(42);
static const comms_id_type ID_DST(666);

SCENARIO("get the source/destination ids", "[comms::message::header]") {
    header h(ID_SRC, ID_DST);
    REQUIRE(ID_SRC == h.get_src());
    REQUIRE(ID_DST == h.get_dst());
}

SCENARIO("header serialization/deserialization", "[comms::message::header]") {
    stringstream ss;
    GIVEN("the header is serialized to a stream") {
        header h(ID_SRC, ID_DST);
        h.serialize(ss);
        REQUIRE(!ss.str().empty());
        WHEN("the stream is deserialized") {
            header h_deserialize(ss);
            THEN("source and destination should be the same") {
                REQUIRE(h.get_src() == h_deserialize.get_src());
                REQUIRE(h.get_dst() == h_deserialize.get_dst());
            }
        }
    }
    GIVEN("the stream is empty") {
        WHEN("the header is deserialized") {
            REQUIRE_THROWS_AS(header h(ss), could_not_deserialize);
        }
    }
}

using cacoon::comms::message::body;

static const string BODY_BASE("H3ll0");
static const string BODY_TO_ADD("world!!");

SCENARIO("adds and retrives data", "[comms::message::body]") {

    GIVEN("an empty message body") {
        body b;
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
        body b(BODY_BASE);
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
        WHEN("the body is deserialized") {
            REQUIRE_THROWS_AS(body b(ss), could_not_deserialize);
        }
    }
    GIVEN("an empty body") {
        body b;
        WHEN("the body is serialized") {
            b.serialize(ss);
            THEN("the stream is deserialized") {
                REQUIRE_THROWS_AS(body b_deserialized(ss), could_not_deserialize);
            }
        }
    }
    GIVEN("a non empty body") {
        body b(BODY_BASE);
        WHEN("the body is serialized") {
            b.serialize(ss);
            THEN("the stream is deserialized") {
                body b_deserialized(ss);
                REQUIRE(b.get_data() == b_deserialized.get_data());
            }
        }
    }
}
