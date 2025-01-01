#include <coroutine>
struct co_getHandle
{
    std::coroutine_handle<> _handle;

    bool await_ready() const noexcept { return false; }
    bool await_suspend(std::coroutine_handle<> handle) noexcept { _handle = handle; return false; }
    std::coroutine_handle<> await_resume() noexcept { return _handle; }
};

// ? get handle with promise_type not tested, but it should work!
template <class promise_type>
struct co_getHandle
{
    std::coroutine_handle<promise_type> _handle;

    bool await_ready() const noexcept { return false; }
    bool await_suspend(std::coroutine_handle<promise_type> handle) noexcept { _handle = handle; return false; }
    std::coroutine_handle<promise_type> await_resume() noexcept { return _handle; }
};

/*
    ? if c++23 or if you have tl::expected
    #include <expected>
    #include <exception>
    #include "unittype.h"
    using expected_ue = ::expected<Unit,std::exception_ptr>;


    template <typename... Args>
    struct std::coroutine_traits<expected_ue, Args...> {
        struct promise_type {
            auto get_return_object() { return std::true_type(); }
            std::suspend_never initial_suspend() { return {}; }
            std::suspend_never final_suspend() noexcept { return {}; }
            void return_void(){}
            void unhandled_exception() {
                std::rethrow_exception(std::current_exception());
            }
        };
    };
*/