
#ifndef CACOON_DATASTORE_H
#define CACOON_DATASTORE_H

#include <string>
#include <map>

namespace cacoon {

    struct datastore {
        using key_type = unsigned int;
        using value_type = std::string;
        using container_type = std::map<key_type, value_type>;

        datastore(const std::string& location);
        ~datastore();

        bool read(const key_type&k, value_type** v);
        bool write(const key_type& k, const value_type& v);
        void to_disk();
        void from_disk();        

    private:
        const static std::string KEY_DELIM_SERIALIZE;
        const static unsigned int KEY_DELIM_DESERIALIZE;

        void serialize();
        void deserialize();

        container_type m_container;
        std::string m_location;
    };
}

#endif 
