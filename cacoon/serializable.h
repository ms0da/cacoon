
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
        //protected:
            virtual void serialize(std::ostream& os) const = 0;
            //virtual void deserialize(std::istream& is) = 0;
            //static void throw_could_not_deserialize();
        };
    }
}

#endif
