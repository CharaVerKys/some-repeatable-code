#pragma once
#include <coroutine>
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
} //nms cvk