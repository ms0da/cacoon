
#ifndef CACOON_COMMS_MESSAGE_FACTORY_H
#define CACOON_COMMS_MESSAGE_FACTORY_H

#include "serializable.h"
#include "../cacoon_test/generated/messages_types_includes.generated.h"
#include "message.h"
#include <map>
#include <memory>

namespace cacoon {
    namespace comms {

        struct message_factory {

            struct serializable_fn {
                using serialize_fn = decltype(&serializable::serialize);
                using deserialize_fn = std::shared_ptr<serializable> (*)(std::istream& is);

                const serialize_fn serialize;
                const deserialize_fn deserialize;
            };

            static const std::map<const cacoon::types::charU8*, serializable_fn> m_map;
        };

    }
}

#endif
