
#include "../ext/catch/catch.hpp"
#include "generated/test_message.generated.h"

using cacoon::comms::messages::test_message;

SCENARIO("can be initialized", "[comms::messages::test_message]") {
    const auto value0 = 8;
    const auto value1 = 10000;
    test_message msg(value0, value1);
    REQUIRE(value0 == msg.value0);
    REQUIRE(value1 == msg.value1);
}

SCENARIO("", "[comms::messages::test_message]") {
      
      
}
