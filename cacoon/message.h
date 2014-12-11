
#ifndef CACOON_COMMS_MESSAGE_H 
#define CACOON_COMMS_MESSAGE_H

#include "transport.h"
#include "serializable.h"

#include <list>
#include <memory>

#include <map>

namespace cacoon {
    namespace comms {

        struct types {
            using charU8 = char;
            using intU8 = charU8;
            using intU32 = unsigned int;
        };

        using comms_id_type = cacoon::transport::id_type;

        struct message : public serializable {
            
            struct header {
                header(std::istream& is);
                header(const comms_id_type& src, const comms_id_type& dst);
                virtual ~header();
                
                const comms_id_type& get_src() const throw();
                const comms_id_type& get_dst() const throw();
                void serialize(std::ostream& os) const;
            private:
                bool deserialize(std::istream& os);

                comms_id_type m_src;
                comms_id_type m_dst;
            };

            struct body {
                body();
                body(std::istream& is);
                virtual ~body();

                bool is_empty() const;
                void add(const serializable&& obj);

                void serialize(std::ostream& os) const;
            private:
                static const char DELIM_START;
                bool deserialize(std::istream& os);
                
                using list_value_type = std::unique_ptr<serializable>;
                std::list<list_value_type> m_list;
            };


            message(std::istream& is);
            message(const comms_id_type& src, const comms_id_type& dst);

            const comms_id_type& get_src() const throw();
            const comms_id_type& get_dst() const throw();

            void append(const serializable&& obj);

            virtual void serialize_type(std::ostream& os) const override;
        private:
            header m_header;
            body m_body;

        };
    }
}

#endif
