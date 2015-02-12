
#ifndef CACOON_COMMON_MODULE_H
#define CACOON_COMMON_MODULE_H

#include <thread>
#include <atomic>
#include <future>

namespace cacoon {
    namespace common {

        struct module {
            using sleep_duration_type = std::chrono::milliseconds;

            module(const sleep_duration_type& sleep_duration = sleep_duration_type(0))
            :m_sleep_duration(sleep_duration) {
                m_run.clear();
            }

            void start() {
                if(!m_run.test_and_set()) {
                    m_main_thread = std::move(std::thread(&module::main_loop, this));
                }
                m_cond_stop.notify_all();
            }

            void stop() {
                m_run.clear();
                if(m_main_thread.joinable()) {
                    m_main_thread.join();
                }
            }
                        
            bool is_running() {
                const bool is_running = m_run.test_and_set();
                if(!is_running) {
                    m_run.clear();
                }
                return is_running;
            }

        protected:
            virtual ~module() {
                stop();
                if(is_running()) {
                    std::unique_lock<std::mutex> lock_stop(m_mutex_stop);
                    m_cond_stop.wait(lock_stop);
                }
            }

        private:

            void main_loop() {
                while(m_run.test_and_set()) {
                    loop();
                    std::this_thread::sleep_for(m_sleep_duration);
                }
                m_run.clear();
            }

            virtual void loop() = 0;
            
            sleep_duration_type m_sleep_duration;
            std::atomic_flag m_run;
            std::thread m_main_thread;
            std::condition_variable m_cond_stop;
            std::mutex m_mutex_stop;
        };

    }
}

#endif
