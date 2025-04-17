#pragma once

#include <coroutine>
#include <exception>
#include <iterator>
#include <optional>
#include <cassert>

namespace cvk{

template<typename T>
class
[[nodiscard("discard generator")]]
generator {
public:
    struct promise_type {
        std::optional<T> current_value;

        generator get_return_object() noexcept {
            return generator{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        std::suspend_always yield_value(T const& value) {
            current_value = value;
            return {};
        }
        std::suspend_always yield_value(T&& value) {
            current_value = std::move(value);
            return {};
        }
        void return_void() noexcept {}
        void unhandled_exception() { std::rethrow_exception(std::current_exception()); }
    };

    struct iterator {
        std::coroutine_handle<promise_type> handle;
        // using difference_type = std::ptrdiff_t;
        // using value_type = T;
        // using iterator_concept = std::input_iterator_tag;

        iterator& operator++() noexcept {
            handle.resume();
            return *this;
        }

        T operator*() const {
            assert(not handle.done());
            return *handle.promise().current_value;
        }

        T* operator->() const {
            assert(not handle.done());
            // yeah also invalidation of pointer after iteration
            return &handle.promise().current_value.value();
        }

        bool operator==(std::default_sentinel_t) const noexcept {
            assert(handle);
            return handle.done();
        }
    };

    generator(generator&& other) noexcept : handle(other.handle) { other.handle = nullptr; }
    void operator=(generator&& other) noexcept {
        if (this not_eq &other) {
            if (handle) {
                handle.destroy();
            }
            handle = other.handle;
            other.handle = nullptr;
        }
    }

    ~generator() { if (handle) handle.destroy(); }

    iterator begin() noexcept {
        if (handle) handle.resume();
        return iterator{handle};
    }

    std::default_sentinel_t end() noexcept { return {}; }

private:
    explicit generator(std::coroutine_handle<promise_type> h) : handle(h) {}
    std::coroutine_handle<promise_type> handle;

    generator(const generator& other)= delete;
    generator& operator=(const generator& other) = delete;
};

}