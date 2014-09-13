
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

        comms_file(const comms_id& location);
        comms_file(const comms_file& c) = delete;
        virtual ~comms_file();

    private:
        virtual content_type update_stream();
        virtual void write_stream(const comms_id& dst, const std::string& str);

        std::ifstream m_read;
    };
    
}

#endif
