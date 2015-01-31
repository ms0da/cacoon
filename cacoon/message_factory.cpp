
#include "message_factory.h"
#include <sstream>
#include <iostream>

using cacoon::comms::message_factory;
using cacoon::comms::serializable;
using std::stringstream;
using std::istringstream;
using std::out_of_range;
using std::end;

std::shared_ptr<message_factory::key_type> message_factory::stream_utility::get_id(std::istream& is) {
    std::shared_ptr<message_factory::key_type> key(std::make_shared<message_factory::key_type>());
    bool id_read;
    types::charU8 id_part[message_factory::key_type::key_len];
    auto id = key.get()->m_key;
    int i = 0;
    do {
        id_read = message_factory::key_type::key_len == 
            is.readsome(id_part, message_factory::key_type::key_len);
        if(id_read) {
            id[i++] = std::stoul(id_part, nullptr, 16);
        }
    } while(id_read && i < message_factory::key_type::key_len);

    
    if(!id_read) {
        key.reset();
    }
    return key;
}

const message_factory::serializable_fn* const message_factory::stream_utility::get_fns(const key_type& key) {
    const serializable_fn* p_elem = nullptr;
    auto itt = m_map.find(key);
    if(end(m_map) != itt) {
        p_elem = &itt->second;
    }
    return p_elem;
}

std::shared_ptr<serializable> message_factory::stream_utility::get_object(std::istream& is, const serializable_fn* const fns) {
    std::shared_ptr<serializable> obj;
    if(nullptr != fns) {
        obj = fns->deserialize(is);
    }
    return obj;
}

std::shared_ptr<serializable> message_factory::stream_utility::deserialize(std::istream& is, const key_type& key) {
	std::shared_ptr<serializable> obj;
    auto fns = get_fns(key);
	if(nullptr != fns) {
        obj = get_object(is, fns);
    }
    return obj;
}

std::shared_ptr<serializable> message_factory::stream_utility::deserialize(std::istream& is, const std::shared_ptr<key_type>& key) {
    return deserialize(is, *key.get());
}

void message_factory::serialize(std::ostream& os, const serializable& obj) {
    serializable::serialize(obj, os);
}

bool message_factory::deserialize(std::istream& is, key_type* key, std::shared_ptr<serializable>* const obj) {
    const auto key_id = stream_utility::get_id(is);
    const auto key_id_ptr = key_id.get();
    if(nullptr != key_id_ptr) {
        const auto fns = stream_utility::get_fns(key_id_ptr->m_key);
        if(nullptr != fns) {
            *obj = fns->deserialize(is);
            *key = key_type(key_id_ptr->m_key);
        }
    }
    return key;
}

