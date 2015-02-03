
#ifndef CACOON_COMMS_MESSAGE_FACTORY_H
#define CACOON_COMMS_MESSAGE_FACTORY_H

#include "serializable.h"
#include <meta/messages/generated/messages_types_includes.generated.h>
#include "message.h"
#include <unordered_map>
#include <memory>
#include <sstream>

using cacoon::comms::types;

namespace cacoon {
    namespace comms {
        struct message_factory {

            struct key_type {
                static const types::intU8 key_len = serializable::id_len;
                types::intU32 m_key[key_len];
                key_type(const types::intU32* k = nullptr) { 
                    const bool is_null = nullptr == k;
                    for (int i = 0; i < key_len; ++i) {
                        m_key[i] = is_null ? 0 : k[i];
                    }
                }                
            };
            
			static void serialize(std::ostream& os, const serializable& obj);
            static bool message_factory::deserialize(std::istream& is, key_type* key, std::shared_ptr<serializable>* const obj);
            using hash_type = std::hash<const key_type>;

            struct serializable_fn {
                using serialize_fn = decltype(&serializable::serialize);
                using deserialize_fn = std::shared_ptr<serializable> (*)(std::istream& is);
                
                const serialize_fn serialize;
                const deserialize_fn deserialize;
            }; 

            // stream utility
            struct stream_utility {
                static std::shared_ptr<key_type> get_id(std::istream& is);
                static const serializable_fn* const get_fns(const key_type& key);
                static std::shared_ptr<serializable> get_object(std::istream& is, const serializable_fn* const fns);
			    static std::shared_ptr<serializable> deserialize(std::istream& is, const key_type& key);
                static std::shared_ptr<serializable> deserialize(std::istream& is, const std::shared_ptr<key_type>& key);
            };
        private:
            struct key_type_compare : public std::binary_function<const key_type, const key_type, bool> {
                bool operator()(const key_type& left, const key_type& right) {
                    bool equal = false;
                    types::intU8 i = 0;
                    do {
                        equal = left.m_key[i] == right.m_key[i];
                        ++i;
                    } while(i < key_type::key_len && equal);
                    return equal;
                }
            };

            struct key_type_hash {
                size_t operator()(const key_type& key) const {
                    size_t hash = 0;
                    types::intU8 i = 0;
                    do {
                        hash ^= key.m_key[i];
                        i++;
                    } while(i < key_type::key_len);
                    return hash;
                }
            };

            using container_type = std::unordered_map<key_type, serializable_fn, key_type_hash, key_type_compare>;
            static const container_type m_map;
        };
    }
}

#endif
