#include <atomic>

namespace cvk{
    class spinlock{
        std::atomic_flag flag = ATOMIC_FLAG_INIT;
      public:
        void lock() {
            while (flag.test_and_set(std::memory_order_acquire)) {}
        }
        void unlock() {
            flag.clear(std::memory_order_release);
        }

        template <typename Rep, typename Period>
        bool lock_for(const std::chrono::duration<Rep, Period>& timeout_duration) {
            auto start = std::chrono::steady_clock::now();
            while (flag.test_and_set(std::memory_order_acquire)) {
                if (std::chrono::steady_clock::now() - start >= timeout_duration) {
                    return false;
                }
            }
            return true;
        }
    };

    class locker{
        cvk::spinlock& sl;
        bool unlocked = false;
      public:
        explicit locker(cvk::spinlock& spinlock)
        :sl(spinlock)
        {
            sl.lock();
        }
        ~locker(){
          if(not unlocked)
            sl.unlock();
        }
        void unlock(){
            sl.unlock();
            unlocked = true;
        }
    };

   ///
   ///

    class external_lock{
        std::atomic_bool locked = false;
      public:
        void lock() {
            locked.store(true,std::memory_order_relaxed);
            while(locked.load(std::memory_order_relaxed));
            std::atomic_thread_fence(std::memory_order_acquire);
        }
        void unlock() {
            cussert(locked.load(std::memory_order_relaxed));
            locked.store(false,std::memory_order_release);
        }

        template <typename Rep, typename Period>
        bool lock_for(const std::chrono::duration<Rep, Period>& timeout_duration) {
            locked.store(true,std::memory_order_relaxed);
            auto start = std::chrono::steady_clock::now();
            while(locked.load(std::memory_order_relaxed)){
                if (std::chrono::steady_clock::now() - start >= timeout_duration) {
                    std::atomic_thread_fence(std::memory_order_acquire); // shall be no-op because data fetch failed
                    return false;
                }
            }
            std::atomic_thread_fence(std::memory_order_acquire);
            return true;
        }
    };



}
