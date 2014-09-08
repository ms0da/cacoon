
#ifndef CACOON_COMMS_H
#define CACOON_COMMS_H

#include <string>
#include <list>
#include <fstream>

namespace cacoon {
    struct comms {
        using comms_id = std::string;

        comms(const comms_id id);
        comms(const comms& c) = delete;
        comms operator=(const comms& c) = delete;
        
        const std::string& get_location() const throw();

        void write(const std::string& dst, const std::string& str);
        void update();
        std::string read();
        bool empty() const throw();
    private:

        using content_type = std::list<std::string>;

        content_type update_stream();

        std::string m_location;
        content_type m_content;
        std::ifstream m_read;
    };
}

#endif
