
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

void assert_message_id(std::istream& msg_ss) {
    message_factory::key_type_array key;
    //if(!message_factory::get_id(msg_ss, key)) {
    //    FAIL("stream is invalid after reading the id");
    //}

    bool id_valid = true;
    for(int i = 0; i < sizeof(key) && id_valid; ++i) {
        id_valid = key[i] == test_message::id[i];
    }
    REQUIRE(id_valid);
}

SCENARIO("can be serialized/deserialized", "[comms::messages::test_message]") {
    const auto value0 = 8;
    const auto value1 = 10000;
    test_message msg(value0, value1);
    REQUIRE(value0 == msg.value0);
    REQUIRE(value1 == msg.value1);

    WHEN("serialization/deserialization is called directly from the class") {
        // serialize
        stringstream msg_ss;
        test_message::serialize(msg, msg_ss);

        // deserialize
        assert_message_id(msg_ss);
        auto msg_deserialized = test_message::deserialize(msg_ss);
        auto msg_deserialized_ptr = static_cast<test_message*>(msg_deserialized.get());
        REQUIRE(msg_deserialized_ptr->value0 == msg.value0);
        REQUIRE(msg_deserialized_ptr->value1 == msg.value1);
    }
    //WHEN("serialization/deserialization is called from the message_factory") {
    //    stringstream msg_ss;
    //    message_factory::serialize(msg_ss, msg);

    //    message_factory::key_type_array id;
    //    bool is_id_extracted = message_factory::get_id(msg_ss, id);
    //    REQUIRE(is_id_extracted);


    //    auto fns = message_factory::get_fns(id);
    //    bool is_null = fns == nullptr;
    //    REQUIRE(!is_null);
    //    auto msg_ptr = message_factory::deserialize(msg_ss, fns);
    //    auto ptr = msg_ptr.get();
    //    is_null = ptr == nullptr;
    //    REQUIRE(!is_null);

    //    auto msg_deserialized_ptr = static_cast<test_message*>(ptr);
    //    REQUIRE(msg_deserialized_ptr->value0 == msg.value0);
    //    REQUIRE(msg_deserialized_ptr->value1 == msg.value1);
    //}
    
}
