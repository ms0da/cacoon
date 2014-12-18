
#ifndef CACOON_COMMS_SERIALIZABLE_H
#define CACOON_COMMS_SERIALIZABLE_H

#include <iosfwd>
#include <exception>
#include <string>
#include <iomanip>

namespace cacoon {
    namespace comms {
        
        namespace exception {
            struct could_not_deserialize : public std::exception {
                could_not_deserialize(const std::string& what);
            };
        }

        struct serializable {
            static const short id_len = 8;
            using id_type = unsigned int[id_len];

            static void serialize(const serializable& s, std::ostream& os) {
                s.serialize_type(os);
            }
        protected:
            static void serialize_id(std::ostream& os, const id_type id) {
                for(unsigned int i = 0; i < id_len; ++i) {
                    os << std::hex 
                        << std::right 
                        << std::setfill('0') 
                        << std::setw(id_len) 
                        << id[i];
                }
            }
        private:
            virtual void serialize_type(std::ostream& os) const {
            };
        };
    }
}

#endif
