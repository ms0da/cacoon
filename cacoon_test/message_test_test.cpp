
#include "../ext/catch/catch.hpp"
#include "generated/test_message.generated.h"
#include "../cacoon//message_factory.h"
#include <sstream>

using cacoon::comms::messages::test_message;
using cacoon::comms::message_factory;
using std::stringstream;
using std::ostringstream;
using std::string;
using std::end;

void consume_message_id(std::istream& msg_ss) {
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
}

SCENARIO("can be serialized/deserialized", "[comms::messages::test_message]") {
    const auto value0 = 8;
    const auto value1 = 10000;
    test_message msg(value0, value1);
    REQUIRE(value0 == msg.value0);
    REQUIRE(value1 == msg.value1);

    WHEN("serialization/deserialization is called directly from the class") 
    {
        ostringstream oss;
        msg.serialize(msg, oss);

        stringstream msg_ss;
        msg_ss << msg.id << "|" << value0 << "|" << value1;
        REQUIRE(oss.str() == msg_ss.str());

        consume_message_id(msg_ss);
        auto msg_deserialized = test_message::deserialize(msg_ss);
        auto msg_deserialized_ptr = static_cast<test_message*>(msg_deserialized.get());
        REQUIRE(msg_deserialized_ptr->value0 == msg.value0);
        REQUIRE(msg_deserialized_ptr->value1 == msg.value1);
    }
    WHEN("serialization/deserialization is called from the message_factory") {
        auto itt = message_factory::m_map.find(test_message::id);
        REQUIRE(itt != end(message_factory::m_map));
        auto fns = itt->second;

        ostringstream oss;
        fns.serialize(msg, oss); 
        stringstream msg_ss;
        msg_ss << msg.id << "|" << msg.value0 << "|" << msg.value1;
        REQUIRE(oss.str() == msg_ss.str());

        consume_message_id(msg_ss);
        auto msg_deserialized = fns.deserialize(msg_ss);
        auto msg_deserialized_ptr = static_cast<test_message*>(msg_deserialized.get());
        REQUIRE(msg_deserialized_ptr->value0 == msg.value0);
        REQUIRE(msg_deserialized_ptr->value1 == msg.value1);
    }
    
}
