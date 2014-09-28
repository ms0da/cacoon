
#ifndef CACOON_AGENT_H
#define CACOON_AGENT_H

#include "comms.h"
#include "datastore.h"
#include <limits>
#include <string>
#include <atomic>
#include <thread>
#include <future>

namespace cacoon {

    static const std::string STORE_LOCATION = "datastore.bin";
    
    namespace agent_id {
        using value_type = unsigned int;
        static const value_type DEFAULT_ID = std::numeric_limits<value_type>::min();
    }

    template<typename T>
    struct agent {
        using transport_type = T;
        using id_type = agent_id::value_type;

        agent(id_type id = agent_id::DEFAULT_ID)
        :m_comms(transport_type(id)), m_id(id), m_store(STORE_LOCATION) {
            m_run.clear();
        }

        void set_id(id_type id) {
            m_id = id;
        }

        id_type get_id() const {
            return m_id;
        }

        void start() {
            //m_thread = std::move(std::thread(&main_loop, this));
            if(!m_run.test_and_set()) {

            }
        }

        bool is_running() {
            const bool is_running = m_run.test_and_set();
            if(!is_running) {
                m_run.clear();
            }
            return is_running;
        }

        void try_stop() {
            m_run.clear();
            //if(m_thread.joinable()) {
            //    m_thread.join();
            //}
        }

    private:

        void main_loop() {
            
        }

        void loop_step() {
            std::cout << "Running0" << std::endl;
            Sleep(5000);
            std::cout << "Running1" << std::endl;
        }

        id_type m_id;
        comms m_comms;
        datastore m_store;

        std::atomic_flag m_run;
        //std::thread m_thread;
        //auto m_future;
    };
}

#endif
