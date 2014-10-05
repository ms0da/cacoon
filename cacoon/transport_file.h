
#ifndef CACOON_COMMS_FILE_H
#define CACOON_COMMS_FILE_H

#include "transport.h"

#include <string>
#include <list>
#include <fstream>

namespace cacoon {

    namespace transport {
        struct file : public impl {
            file(const id_type& id);
            file(const file& c) = delete;
            virtual ~file();

        private:
            virtual content_type receive_stream();
            virtual void send_stream(const id_type& dst, const std::string& str);

            std::ifstream m_read;
        };
    }
    
}

#endif
