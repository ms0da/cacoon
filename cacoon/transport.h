
#ifndef CACOON_comms_H
#define CACOON_comms_H

#include <string>
#include <list>
#include <memory>

namespace cacoon {

    namespace transport_types {
        using transport_id = unsigned int;
        using content_type = std::list<std::string>;
    }

    struct transport_impl {
        using transport_id = transport_types::transport_id;
        using content_type = transport_types::content_type;

        transport_impl(const transport_id& id);
        transport_impl(const transport_impl& c) = delete;
        transport_impl(transport_impl&& c) = delete;
        transport_impl& operator=(transport_impl&& c) = delete;

        virtual content_type receive_stream() = 0;
        virtual void send_stream(const transport_id& dst, const std::string& str) = 0;
        const transport_id& get_location() const throw();

    private:
        transport_id m_id;
    };

    struct transport {
        using transport_id = transport_types::transport_id;
        using content_type = transport_types::content_type;

        transport(transport_impl&& impl);
        transport(const transport& c) = delete;
        virtual ~transport();

        void send(const transport_id& dst, const std::string& str);
        void receive();
        const std::string get_string();
        bool empty() const throw();
        const transport_id& get_location() const throw();        
    private:
        content_type m_content;
        transport_impl* m_impl;
        //std::unique_ptr<comms_impl> m_impl;
    };

}

#endif
