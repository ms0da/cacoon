
#ifndef CACOON_COMMS_H
#define CACOON_COMMS_H

#include "module.h"
#include "message.h"
#include "serializable.h"
#include <list>
#include <memory>
#include <iosfwd>

namespace cacoon {
    namespace comms {

        struct comms : public cacoon::common::module {

            comms() {
                this->init();
            }

            virtual ~comms() {
            }

        };

    }
}

#endif
