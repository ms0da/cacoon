
#include "../ext/catch/catch.hpp"
#include "meta/messages/generated/test_message.generated.h"
#include "../cacoon//message_factory.h"
#include <sstream>

using cacoon::comms::messages::test_message;
using cacoon::comms::message_factory;
using std::stringstream;
using std::ostringstream;
using std::istream;
using std::string;
using std::end;

bool validate_key(const types::intU32 key_read[], const types::intU32 message_key[]) {
    bool is_valid = true;
    for(int i = 0; is_valid && i < message_factory::key_type::key_len; ++i) {
        is_valid = key_read[i] == message_key[i];
    }
    return is_valid;
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
        auto key = message_factory::stream_utility::get_id(msg_ss);
        auto key_ptr = key.get();
        bool id_read = nullptr != key_ptr;
        REQUIRE(id_read);

        auto key_read = key_ptr->m_key;
        REQUIRE(validate_key(key_read, test_message::id));

        auto msg_deserialized = test_message::deserialize(msg_ss);
        auto msg_deserialized_ptr = static_cast<test_message*>(msg_deserialized.get());
        REQUIRE(msg_deserialized_ptr->value0 == msg.value0);
        REQUIRE(msg_deserialized_ptr->value1 == msg.value1);
    }
    WHEN("serialization/deserialization is called from the message_factory") {
        // serialize
		stringstream msg_ss;
        message_factory::serialize(msg_ss, msg);

		// deserialize
        auto key = message_factory::stream_utility::get_id(msg_ss);
        auto key_ptr = key.get();
        bool id_read = nullptr != key_ptr;
        REQUIRE(id_read);

        auto key_read = key_ptr->m_key;
        REQUIRE(validate_key(key_read, test_message::id));

        auto fns = message_factory::stream_utility::get_fns(*key_ptr);
        bool is_null = nullptr == fns;
        REQUIRE(!is_null);

        auto msg_deserialized = fns->deserialize(msg_ss);
        auto msg_deserialized_ptr = static_cast<test_message*>(msg_deserialized.get());
        REQUIRE(msg_deserialized_ptr->value0 == msg.value0);
        REQUIRE(msg_deserialized_ptr->value1 == msg.value1);
    }
    WHEN("serialization/deserialization right from the message_factory") {
        // serialize
		stringstream msg_ss;
        message_factory::serialize(msg_ss, msg);

        // deserialize
        std::shared_ptr<cacoon::comms::serializable> obj;
        message_factory::key_type key;
        bool is_deserialized = message_factory::deserialize(msg_ss, &key, &obj);
        REQUIRE(is_deserialized);
        is_deserialized = nullptr != key.m_key;
        REQUIRE(is_deserialized);
        REQUIRE(validate_key(key.m_key, test_message::id));
        
        auto msg_deserialized_ptr = static_cast<test_message*>(obj.get());
        REQUIRE(msg_deserialized_ptr->value0 == msg.value0);
        REQUIRE(msg_deserialized_ptr->value1 == msg.value1);
    }
}
