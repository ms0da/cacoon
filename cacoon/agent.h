
#ifndef CACOON_AGENT_H
#define CACOON_AGENT_H

#include "transport.h"
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

    template<typename transport_type>
    struct agent {
        using id_type = agent_id::value_type;

        agent(id_type id = agent_id::DEFAULT_ID)
        :m_transport(T(id)), m_id(id), m_store(STORE_LOCATION) {
            m_run.clear();
        }

        ~agent() {
            try_stop();
            if(is_running()) {
                std::unique_lock<std::mutex> lock_stop(m_mutex_stop);
                m_cond_stop.wait(lock_stop);
            }
        }

        void set_id(id_type id) {
            m_id = id;
        }

        id_type get_id() const {
            return m_id;
        }

        void start() {
            if(!m_run.test_and_set()) {
                m_main_thread = std::move(std::thread(&agent::main_loop, this));
            }
            m_cond_stop.notify_all();
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
            if(m_main_thread.joinable()) {
                m_main_thread.join();
            }
        }

    private:

        void main_loop() {
            while(m_run.test_and_set()) {
                // get message from comms
                // execute on a thread (package_task)
                // send result from action
            }
            m_run.clear();
        }
        id_type m_id;
        transport::base m_transport;
        datastore m_store;

        std::atomic_flag m_run;
        std::thread m_main_thread;

        std::mutex m_mutex_stop;
        std::condition_variable m_cond_stop;
        //auto m_future;
    };
}

#endif
