
#ifndef CACOON_COMMS_MESSAGE_FACTORY_H
#define CACOON_COMMS_MESSAGE_FACTORY_H

#include "serializable.h"
#include "../cacoon_test/generated/messages_types_includes.generated.h"
#include "message.h"
#include <unordered_map>
#include <memory>
#include <sstream>

using cacoon::comms::types;

namespace cacoon {
    namespace comms {
        struct message_factory {

            //using key_type = types::charU8*;
            //using const_key_type = const types::charU8*;
            static const types::intU8 key_len = 65;
            using key_type_array = types::charU8[key_len - 1];

            struct key_type {
                const types::charU8* m_key;
                key_type(const types::charU8* k = nullptr) { m_key = k; }
            };

            struct serializable_fn {
                using serialize_fn = decltype(&serializable::serialize);
                using deserialize_fn = std::shared_ptr<serializable> (*)(std::istream& is);
                
                const serialize_fn serialize;
                const deserialize_fn deserialize;
            }; 

            // stream utility
            static bool get_id(std::istream& is, key_type& key);

            //static const serializable_fn* const get_fns(const_key_type key);
            static std::shared_ptr<serializable> deserialize(std::istream& is, const serializable_fn* const fns);
            static void serialize(std::ostream& os, const serializable& obj);

            using hash_type = std::hash<const key_type>;

        private:
            struct key_type_compare : public std::binary_function<const key_type, const key_type, bool> {
                bool operator()(const key_type& left, const key_type& right) {
                    bool equal = true;
                    for(types::intU8 i = 0; i < key_len - 1 && equal; ++i) {
                        equal = left.m_key[i] == right.m_key[i];
                    }
                    return equal;
                }
            };

            size_t key_type_hash(key_type& key) {
                std::stringstream ss;
                ss << key.m_key;
                long x;
                ss >> x;


                return std::hash<decltype(key.m_key)>()(key.m_key);
            }


            using container_type = std::unordered_map<key_type, serializable_fn, decltype(&key_type_hash), key_type_compare>;
            static const container_type m_map;
        };
    }
}

#endif
