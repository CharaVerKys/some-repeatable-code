#pragma once
#include <coroutine>
#include <iostream>
namespace cvk{
struct co_getHandle
{
    std::coroutine_handle<> _handle;

    bool await_ready() const noexcept { return false; }
    bool await_suspend(std::coroutine_handle<> handle) noexcept { _handle = handle; return false; }
    [[nodiscard]] std::coroutine_handle<> await_resume() noexcept { return _handle; }
};

// ? get handle with promise_type not tested, but it should work!
template <class promise_type>
struct co_getHandleT // T mean templated
{
    std::coroutine_handle<promise_type> _handle;

    bool await_ready() const noexcept { return false; }
    bool await_suspend(std::coroutine_handle<promise_type> handle) noexcept { _handle = handle; return false; }
    [[nodiscard]] std::coroutine_handle<promise_type> await_resume() noexcept { return _handle; }
};
struct coroutine_t{};
template <typename... Args>
concept NoReferenceArgs = 
    (sizeof...(Args) == 0) or
    []<typename Class, typename... Rest>() {
        return ((not std::is_reference_v<Rest>) and ...);
    }.template operator()<Args...>();
} //nms cvk



#include <exception>
template <typename... Args>
struct std::coroutine_traits<cvk::coroutine_t, Args...> {
    static_assert(cvk::NoReferenceArgs<Args...>,
              "Cannot use references for non-scope lifetime coroutine");
    struct promise_type {
        cvk::coroutine_t get_return_object(){return {};}
        std::suspend_never initial_suspend() { return {}; }
        std::suspend_never final_suspend() noexcept { return {}; }
        void return_void(){}
        void unhandled_exception(){
            std::terminate();
        }
    };
};