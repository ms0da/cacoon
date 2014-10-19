
#ifndef CACOON_COMMS_MESSAGE_H 
#define CACOON_COMMS_MESSAGE_H

#include "transport.h"
#include "serializable.h"

namespace cacoon {
    namespace comms {
        
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
                body(std::istream& is);
                body(const std::string& data = "");
                virtual ~body();

                void add_data(const std::string& data);
                std::string get_data() const throw();
                void serialize(std::ostream& os) const;
            private:
                static const char DELIM_START;
                bool deserialize(std::istream& os);

                std::string m_data;
            };


            message(std::istream& is);
            message(const comms_id_type& src, const comms_id_type& dst);

            const comms_id_type& get_src() const throw();
            const comms_id_type& get_dst() const throw();

            void append(const serializable& obj);

            virtual void serialize(std::ostream& os) const override;
        private:
            header m_header;
            body m_body;

        };
    }
}

#endif
