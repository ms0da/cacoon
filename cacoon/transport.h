
#ifndef CACOON_comms_H
#define CACOON_comms_H

#include <string>
#include <list>
#include <memory>

namespace cacoon {
    namespace transport {
        using id_type = unsigned int;
        using content_type = std::list<std::string>;

        struct impl {
            impl(const id_type& id);
            impl(const impl& c) = delete;
            impl(impl&& c) = delete;
            impl& operator=(impl&& c) = delete;

            virtual content_type receive_stream() = 0;
            virtual void send_stream(const id_type& dst, const std::string& str) = 0;
            const id_type& get_id() const throw();

        private:
            id_type m_id;
        };

        struct base {
            base(impl&& impl);
            base(const base& c) = delete;
            virtual ~base();

            void send(const id_type& dst, const std::string& str);
            void receive();
            const std::string get_string();
            bool empty() const throw();
            const id_type& get_id() const throw();        
        private:
            content_type m_content;
            impl* m_impl;
        };
    }
}

#endif
