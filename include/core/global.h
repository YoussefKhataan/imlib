#pragma once
#include <atomic>

namespace imlib
{  
    class GlobalConfig {
    private:
        static std::atomic<bool>& get_flag_instance() {
            static std::atomic<bool> flag(false); 
            return flag;
        }

    public:
        static inline void turn_gpu_on() {
            get_flag_instance().store(true);
        }

        static inline void turn_gpu_off() {
            get_flag_instance().store(false);
        }

        static inline bool is_gpu_on() {
            return get_flag_instance().load();
        }
    };
}