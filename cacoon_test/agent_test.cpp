
#include "cacoon_test.h"
#include "../cacoon/transport_file.h"
#include "../cacoon/agent.h"

using cacoon::agent;
using cacoon::agent_id::value_type;
using cacoon::transport_file;
using std::move;

SCENARIO("give and retrieve the id of an agent", "[agent]") {  
    using agent_type = agent<transport_file>;
    agent_type agent;
    REQUIRE(cacoon::agent_id::DEFAULT_ID == agent.get_id());

    GIVEN("a value is given to the id") {
        const agent_type::id_type new_id = 42;
        agent.set_id(new_id);

        WHEN("the new value of id can be retrieved and should match") {
            REQUIRE(new_id == agent.get_id());
        }
    }
}
