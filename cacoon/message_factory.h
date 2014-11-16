
#ifndef CACOON_COMMS_MESSAGE_FACTORY_H
#define CACOON_COMMS_MESSAGE_FACTORY_H

#include "serializable.h"
#include "../cacoon_test/messages_definitions/messages_types_includes.h"
#include <map>

namespace cacoon {
    namespace comms {

        struct message_factory {

            struct serializable_fn {
                using serialize_fn = decltype(&serializable::serialize);
                using deserialize_fn = std::shared_ptr<serializable> (*)(std::istream& is);

                const serialize_fn serialize;
                const deserialize_fn deserialize;
            };

            static const std::map<const types::charU8*, serializable_fn> m_map;
        };

    }
}

#endif
