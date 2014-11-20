
#ifndef CACOON_COMMS_SERIALIZABLE_H
#define CACOON_COMMS_SERIALIZABLE_H

#include <iosfwd>
#include <exception>
#include <string>

namespace cacoon {
    namespace comms {
        
        namespace exception {
            struct could_not_deserialize : public std::exception {
                could_not_deserialize(const std::string& what);
            };
        }

        struct serializable {
            static void serialize(const serializable& s, std::ostream& os) {
                s.serialize_type(os);
            }
        private:
            virtual void serialize_type(std::ostream& os) const {
            };
        };
    }
}

#endif
