
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

    //struct transport_file : public transport_impl {
    //    using transport_id = transport::transport_id;
    //    using content_type = transport::content_type;

    //    transport_file(const transport_id& id);
    //    transport_file(const transport_file& c) = delete;
    //    virtual ~transport_file();

    //private:
    //    virtual content_type receive_stream();
    //    virtual void send_stream(const transport_id& dst, const std::string& str);

    //    std::ifstream m_read;
    //};
    
}

#endif
