
#ifndef CACOON_COMMS_FILE_H
#define CACOON_COMMS_FILE_H

#include "comms.h"

#include <string>
#include <list>
#include <fstream>

namespace cacoon {

    struct comms_file : public comms_impl {
        using comms_id = comms::comms_id;
        using content_type = comms::content_type;

        comms_file(const comms_id& id);
        comms_file(const comms_file& c) = delete;
        virtual ~comms_file();

    private:
        virtual content_type receive_stream();
        virtual void send_stream(const comms_id& dst, const std::string& str);

        std::ifstream m_read;
    };
    
}

#endif
