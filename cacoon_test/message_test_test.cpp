
#include "../ext/catch/catch.hpp"
#include "generated/test_message.generated.h"
#include <sstream>

using cacoon::comms::messages::test_message;
using std::stringstream;
using std::ostringstream;
using std::string;

SCENARIO("can be serialized/deserialized", "[comms::messages::test_message]") {
    const auto value0 = 8;
    const auto value1 = 10000;
    test_message msg(value0, value1);
    REQUIRE(value0 == msg.value0);
    REQUIRE(value1 == msg.value1);

    ostringstream oss;
    msg.serialize(msg, oss);

    stringstream msg_ss;
    msg_ss << msg.id << "|" << value0 << "|" << value1;
    REQUIRE(oss.str() == msg_ss.str());

    char str_id[64];
    bool stream_valid = true;
    if(!msg_ss.readsome(str_id, sizeof(str_id))) {
        FAIL("stream is invalid after reading the id");
    }
    
    bool id_valid = true;
    for(int i = 0; i < sizeof(str_id) && id_valid; ++i) {
        id_valid = str_id[i] == test_message::id[i];
    }
    REQUIRE(id_valid);

    auto msg_deserialized = test_message::deserialize(msg_ss);
    auto msg_deserialized_ptr = static_cast<test_message*>(msg_deserialized.get());
    REQUIRE(msg_deserialized_ptr->value0 == msg.value0);
    REQUIRE(msg_deserialized_ptr->value1 == msg.value1);
}
