
#include "../ext/catch/catch.hpp"
#include "test_utils.h"

#include "../cacoon/datastore.h"

using cacoon::datastore;
using std::string;

static const string STORE_FILENAME("datastore.bin");
static const datastore::key_type KEY0(0);
static const datastore::key_type KEY1(1);
static const datastore::key_type KEY2(2);
static const datastore::value_type VALUE0("value0");
static const datastore::value_type VALUE1("value1");

SCENARIO("can read/write values", "[datastore]") {
    test_utils::empty_file(STORE_FILENAME);
    datastore store(STORE_FILENAME);
    datastore::value_type* pValue(nullptr);

    GIVEN("without any values") {
        bool is_read = store.read(KEY0, &pValue);
        REQUIRE(!is_read);
    }
    GIVEN("with single value") {
        //bool is_written = store.write(KEY0, VALUE0);
        REQUIRE(store.write(KEY0, VALUE0));

        //bool is_read = store.read(KEY0, &pValue);
        REQUIRE(store.read(KEY0, &pValue));
        REQUIRE(*pValue == VALUE0);
        //is_read = store.read(KEY1, &pValue);
        REQUIRE(!store.read(KEY1, &pValue));
    }
    GIVEN("with multiple values") {
        //bool is_written = store.write(KEY0, VALUE0);
        REQUIRE(store.write(KEY0, VALUE0));
        //is_written = store.write(KEY0, VALUE1);
        REQUIRE(!store.write(KEY0, VALUE1));
        //is_written = store.write(KEY1, VALUE0);
        REQUIRE(store.write(KEY1, VALUE0));
        //is_written = store.write(KEY1, VALUE1);
        REQUIRE(!store.write(KEY1, VALUE1));
        
        bool is_read = store.read(KEY0, &pValue);
        REQUIRE(is_read);
        REQUIRE(*pValue == VALUE0);
        is_read = store.read(KEY1, &pValue);
        REQUIRE(is_read);
        REQUIRE(*pValue == VALUE0);
        is_read = store.read(KEY2, &pValue);
        REQUIRE(!is_read);
        store.to_disk();
    }
}

SCENARIO("data is load from disk on construction and written to disk on deletion", "[datastore]") {
    test_utils::empty_file(STORE_FILENAME);
    datastore* store0 = new datastore(STORE_FILENAME);
    REQUIRE(store0->write(KEY1, VALUE1));
    delete store0;

    datastore store1(STORE_FILENAME);
    datastore::value_type* pValue(nullptr);
    REQUIRE(!store1.read(KEY0, &pValue));
    REQUIRE(store1.read(KEY1, &pValue));
    REQUIRE(*pValue == VALUE1);
}

SCENARIO("data persistence can be forced", "[datastore]") {
    test_utils::empty_file(STORE_FILENAME);
    datastore store0(STORE_FILENAME);
    store0.from_disk();

    datastore::value_type* pValue0(nullptr);
    REQUIRE(!store0.read(KEY0, &pValue0));
    REQUIRE(store0.write(KEY0, VALUE0));
    
    datastore store1(STORE_FILENAME);
    datastore::value_type* pValue1(nullptr);
    REQUIRE(!store1.read(KEY0, &pValue1));

    store0.to_disk();
    store1.from_disk();

    REQUIRE(store1.read(KEY0, &pValue1));
    REQUIRE(*pValue1 == VALUE0);
}
