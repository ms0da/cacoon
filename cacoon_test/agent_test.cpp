
#include "cacoon_test.h"
#include "../cacoon/transport_file.h"
#include "../cacoon/agent.h"

using cacoon::agent;
using cacoon::agent_id::value_type;
using cacoon::transport::file;
using std::move;

using agent_file_type = agent<file>;

SCENARIO("give and retrieve the id of an agent", "[agent]") {  
    agent_file_type agent;
    REQUIRE(cacoon::agent_id::DEFAULT_ID == agent.get_id());

    GIVEN("a value is given to the id") {
        const agent_file_type::id_type new_id = 42;
        agent.set_id(new_id);

        WHEN("the new value of id can be retrieved and should match") {
            REQUIRE(new_id == agent.get_id());
        }
    }
}

SCENARIO("start and stop", "[agent]") {
    agent_file_type agent;

    GIVEN("a new agent") {
        THEN("the agent should not be running") {
            REQUIRE(!agent.is_running());    
        }
    }
    GIVEN("the agent starts") {
        agent.start();
        THEN("the agent should be running") {
            REQUIRE(agent.is_running());
            AND_WHEN("the agent is stopped") {
                agent.try_stop();
                THEN("the agent is not running anymore") {
                    REQUIRE(!agent.is_running());
                }
            }
        }
    }
}
