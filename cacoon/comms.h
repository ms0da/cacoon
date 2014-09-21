
#ifndef CACOON_comms_H
#define CACOON_comms_H

#include <string>
#include <list>
#include <memory>

namespace cacoon {

    namespace comms_types {
        using comms_id = unsigned int;
        using content_type = std::list<std::string>;
    }

    struct comms_impl {
        using comms_id = comms_types::comms_id;
        using content_type = comms_types::content_type;

        comms_impl(const comms_id& id);
        comms_impl(const comms_impl& c) = delete;
        comms_impl(comms_impl&& c) = delete;
        comms_impl& operator=(comms_impl&& c) = delete;

        virtual content_type receive_stream() = 0;
        virtual void send_stream(const comms_id& dst, const std::string& str) = 0;
        const comms_id& get_location() const throw();

    private:
        comms_id m_id;
    };

    struct comms {
        using comms_id = comms_types::comms_id;
        using content_type = comms_types::content_type;

        comms(comms_impl&& impl);
        comms(const comms& c) = delete;
        virtual ~comms();

        void send(const comms_id& dst, const std::string& str);
        void receive();
        const std::string get_string();
        bool empty() const throw();
        const comms_id& get_location() const throw();        
    private:
        content_type m_content;
        comms_impl* m_impl;
        //std::unique_ptr<comms_impl> m_impl;
    };

}

#endif
