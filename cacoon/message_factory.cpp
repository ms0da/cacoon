
#include "message_factory.h"
#include <sstream>

using cacoon::comms::message_factory;
using cacoon::comms::serializable;
using std::istringstream;
using std::out_of_range;
using std::end;


#include <iostream>
#include "../cacoon_test/generated/test_message.generated.h"

bool message_factory::get_id(std::istream& is, key_type& key) {
    
    //bool id_read = key_len - 1 == is.readsome(key.m_key, key_len - 1);
    bool id_read = false;
    if(id_read) {
        //auto key_hash = m_map.hash_function()(key);  
        //auto key_hash_msg = m_map.hash_function()(cacoon::comms::messages::test_message::id);

        //auto b = m_map.bucket(key);
        //auto b_hash = m_map.bucket(cacoon::comms::messages::test_message::id);

        //std::cout << std::endl << b << std::endl;
    }
    return id_read;
}

//const message_factory::serializable_fn* const message_factory::get_fns(const_key_type key) {
//    const serializable_fn* p_elem = nullptr;
//    auto itt = m_map.find(key);
//    if(end(m_map) != itt) {
//        p_elem = &itt->second;
//    }
//    return p_elem;
//}

std::shared_ptr<serializable> message_factory::deserialize(std::istream& is, const serializable_fn* const fns) {
    std::shared_ptr<serializable> obj;
    if(nullptr != fns) {
        obj = fns->deserialize(is);
    }
    return obj;
}

void message_factory::serialize(std::ostream& os, const serializable& obj) {
    serializable::serialize(obj, os);
}
