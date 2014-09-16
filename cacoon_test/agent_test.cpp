
#include "cacoon_test.h"
#include "../cacoon/comms_file.h"
#include "../cacoon/agent.h"

using cacoon::agent;
using cacoon::comms_file;
using std::move;

SCENARIO("give and retrieve the id of an agent", "[agent]") {
    comms_file::comms_id id("agent_id");
    comms_file comms(id);
    agent agent(move(comms));
    REQUIRE(agent::DEFAULT_ID == agent.get_id());

    GIVEN("a value is given to the id") {
        const agent::id_type new_id = 42;
        agent.set_id(new_id);

        WHEN("the new value of id can be retrieved and should match") {
            REQUIRE(new_id == agent.get_id());
        }
    }
}