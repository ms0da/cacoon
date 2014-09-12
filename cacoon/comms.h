
#ifndef CACOON_comms_H
#define CACOON_comms_H

#include <string>
#include <list>

namespace cacoon {

    struct comms {
        using comms_id = std::string;

        comms(const comms_id& location);
        comms(const comms& c) = delete;
        virtual ~comms();

        void write(const comms_id& dst, const std::string& str);
        void update();
        const std::string read();
        bool empty() const throw();
        const std::string& get_location() const throw();

    protected:
        using content_type = std::list<std::string>;

    private:
        virtual content_type update_stream() = 0;
        virtual void write_stream(const comms_id& dst, const std::string& str) = 0;

        comms_id m_location;
        content_type m_content;
    };

}

#endif
