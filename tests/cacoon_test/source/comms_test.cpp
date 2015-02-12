
#include <catch/catch.hpp>
#include <cacoon/include/engine.h>
#include <cacoon/include/transport_file.h>

using cacoon::comms::engine;

SCENARIO("engine start and stop", "[comms::engine]") {
    using module_type = engine;
    
    cacoon::transport::file impl(cacoon::transport::id_type(0));
    cacoon::transport::base base(std::move(impl));
    module_type m(base);

    GIVEN("a new engine") {
        THEN("the engine should not be running") {
            REQUIRE(!m.is_running());    
        }
    }
    GIVEN("the engine starts") {
        m.start();
        THEN("the engine should be running") {
            REQUIRE(m.is_running());
            AND_WHEN("the engine is stopped") {
                m.stop();
                THEN("the engine is not running anymore") {
                    REQUIRE(!m.is_running());
                }
            }
        }
    }
}
