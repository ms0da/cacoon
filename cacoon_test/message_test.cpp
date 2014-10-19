
#include "../ext/catch/catch.hpp"
#include "../cacoon/message.h"

#include <string>
#include <iosfwd>

using cacoon::comms::message;
using cacoon::comms::comms_id_type;
using cacoon::comms::serializable;
using cacoon::comms::exception::could_not_deserialize;

using std::string;
using std::stringstream;


static const comms_id_type ID_SRC(123);
static const comms_id_type ID_DST(456);


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


}

        /*AND_WHEN("data is added to the message") {
            message msg_to_add(ID_DST, ID_SRC);
            msg.append(msg);


            
        }*/
