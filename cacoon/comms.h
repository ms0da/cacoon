
#ifndef CACOON_COMMS_H
#define CACOON_COMMS_H

#include "transport.h"
#include <string>
#include <ostream>

namespace cacoon {
    namespace comms {
        using comms_id_type = cacoon::transport::id_type;

        namespace exception {
            struct could_not_deserialize : public std::exception {
                could_not_deserialize(const std::string& what)
                    :std::exception(what.c_str()) {}
            };
        }

        struct serializable {
            virtual void serialize(std::ostream& os) = 0;
            virtual void deserialize(std::istream& is) = 0;
            static void throw_could_not_deserialize();
        };
        
        namespace message {

            struct header : public serializable {
                header(std::istream& is);
                header(const comms_id_type& src, const comms_id_type& dst);
                virtual ~header();
                
                const comms_id_type& get_src() const throw();
                const comms_id_type& get_dst() const throw();
                virtual void serialize(std::ostream& os) override;
                virtual void deserialize(std::istream& os) override;
            private:
                comms_id_type m_src;
                comms_id_type m_dst;
            };

            struct body : public serializable {
                body(std::istream& is);
                body(const std::string& data = "");
                virtual ~body();

                void add_data(const std::string& data);
                std::string get_data() const throw();
                virtual void serialize(std::ostream& os) override;
                virtual void deserialize(std::istream& os) override;
            private:
                std::string m_data;
            };

        }

    }
}

#endif
