
#include "cacoon_test.h"

#include "../cacoon/agent.h"
using cacoon::agent;

SCENARIO("give and retrieve the id of an agent", "[agent]") {
    agent agent;
    REQUIRE(agent::DEFAULT_ID == agent.get_id());

    GIVEN("a value is given to the id") {
        const agent::id_type new_id = 42;
        agent.set_id(new_id);

        WHEN("the new value of id can be retrieved and should match") {
            REQUIRE(new_id == agent.get_id());
        }
    }
}