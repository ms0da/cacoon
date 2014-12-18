
#include "message_factory.h"
#include <sstream>
#include <iostream>

using cacoon::comms::message_factory;
using cacoon::comms::serializable;
using std::stringstream;
using std::istringstream;
using std::out_of_range;
using std::end;

/*
types::intU32 key[message_factory::key_type::key_len];
    types::charU8 key_part[message_factory::key_type::key_len];
    for(int i = 0; i < message_factory::key_type::key_len; ++i) {
        stringstream ss;
        msg_ss.read(key_part, message_factory::key_type::key_len);
        ss << key_part;
        ss >> std::hex >> key[i];
    }
    
    bool id_valid = true;
    for(int i = 0; i < message_factory::key_type::key_len && id_valid; ++i) {
        id_valid = test_message::id[i] == key[i];
    }
*/

//bool message_factory::get_id(std::istream& is, key_type& key) {
//    bool id_read;
//    types::charU8 id_part[message_factory::key_type::key_len];
//    serializable::id_type id;
//    stringstream ss;
//    int i = 0;
//    do {
//        id_read = message_factory::key_type::key_len == 
//            is.readsome(id_part, message_factory::key_type::key_len);
//        if(id_read) {
//            ss << id_part;
//            ss >> std::hex >> id[i++];
//        }
//    } while (id_read && i < message_factory::key_type::key_len);
//
//    if(id_read) {
//    }
//    return id_read;
//}


std::shared_ptr<message_factory::key_type> message_factory::get_id(std::istream& is) {
    bool id_read;
    types::charU8 id_part[message_factory::key_type::key_len];
    serializable::id_type id;
    stringstream ss;
    int i = 0;
    do {
        id_read = message_factory::key_type::key_len == 
            is.readsome(id_part, message_factory::key_type::key_len);
        if(id_read) {
            ss << id_part;
            ss >> std::hex >> id[i++];
        }
    } while (id_read && i < message_factory::key_type::key_len);

    std::shared_ptr<message_factory::key_type> key;
    if(id_read) {
        key = std::make_shared<message_factory::key_type>(id);
    }
    return key;
}

const message_factory::serializable_fn* const message_factory::get_fns(const key_type& key) {
    const serializable_fn* p_elem = nullptr;
    auto itt = m_map.find(key);
    if(end(m_map) != itt) {
        p_elem = &itt->second;
    }
    return p_elem;
}

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
