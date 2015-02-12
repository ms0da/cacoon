
#include <catch/catch.hpp>
#include <cacoon/include/message.h>
#include <cacoon/include/message_factory.h>

#include <string>
#include <iosfwd>

using cacoon::comms::message;
using cacoon::comms::comms_id_type;
using cacoon::comms::serializable;
using cacoon::comms::exception::could_not_deserialize;

using std::string;
using std::stringstream;

static const comms_id_type ID_SRC(123);
static const comms_id_type ID_SRC1(42);
static const comms_id_type ID_DST(456);
static const comms_id_type ID_DST1(24);
static const string BODY_BASE("H3ll0");
static const string BODY_TO_ADD(" world!!");



SCENARIO("the message type exists", "[comms::message_factory]") {
    


    /*
    auto x = cacoon::comms::message_factory::m_map.at(0);
    auto msg = cacoon::comms::test_message(1, 2);
    x.deserialize(std::cin);
    x.serialize(msg, std::cout);
    */
}

/*
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
        REQUIRE(b.is_empty());

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
        REQUIRE(!b.is_empty());
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


SCENARIO("initialization", "[comms::message]") {
    WHEN("source and destination are given") {
        message msg(ID_SRC, ID_DST);
        THEN("values retrieves are expected to match") {
            REQUIRE(msg.get_src() == ID_SRC);
            REQUIRE(msg.get_dst() == ID_DST);
        }
    }
}

SCENARIO("serialization/deserialization", "[comms::message]") {
    stringstream ss;
    WHEN("the stream is empty") {
        THEN("the message throws an exception") {
            REQUIRE_THROWS_AS(message msg(ss), could_not_deserialize);
        }
    }
    WHEN("the stream contains only a header") {
        message::header h(ID_SRC, ID_DST);
        h.serialize(ss);
        AND_WHEN("the message cannot be rebuilt from the stream") {
            REQUIRE_THROWS_AS(message msg(ss), could_not_deserialize);
        }
    }
    WHEN("the stream contains only a body") {
        message::body b(BODY_BASE);
        b.serialize(ss);
        AND_WHEN("the message cannot be rebuilt from the stream") {
            REQUIRE_THROWS_AS(message msg(ss), could_not_deserialize);
        }
    }
    WHEN("data is added to a message") {
        message msg(ID_SRC, ID_DST);
        message msg_to_add(ID_SRC1, ID_DST1);
        //msg.append(

    }
}

        /*AND_WHEN("data is added to the message") {
            message msg_to_add(ID_DST, ID_SRC);
            msg.append(msg);


            
        }*/

