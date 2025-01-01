#ifndef DefaultCoroutine_H
#define DefaultCoroutine_H

#include <iostream>
#include <coroutine>
#include <cassert>

class [[nodiscard("discard coroutine!!")]] DefaultCoroutine 
{
public:
    struct promise_type {
        DefaultCoroutine get_return_object() {
            return DefaultCoroutine(std::coroutine_handle<promise_type>::from_promise(*this));
        }

        std::suspend_always initial_suspend() noexcept {
            return std::suspend_always();
        }

        std::suspend_always final_suspend() noexcept {
            return std::suspend_always();
        }

        void return_void() noexcept {}

        [[noreturn]] void unhandled_exception() {
            std::cerr << "unhandled_exception in DefaultCoroutine."  <<std::endl;
            try{
                std::rethrow_exception(std::current_exception());
            }catch(std::exception&e){
                std::cout << e.what()<<std::endl;
            }
            std::terminate();
        }
    };

public:
    DefaultCoroutine(std::coroutine_handle<promise_type> h = nullptr):handle(h){}
    ~DefaultCoroutine(){if(handle){handle.destroy();} }
    [[nodiscard]] bool resume() noexcept{ // ? может пометить nodiscard... в будущем для улучшения надёжности
        assert(handle);
        if (handle && not handle.done()) {
            handle.resume();
            return true;
        }
        return false;
    }
    DefaultCoroutine(const DefaultCoroutine& other)= delete;
    DefaultCoroutine& operator=(const DefaultCoroutine& other) = delete;

    DefaultCoroutine(DefaultCoroutine&& other) noexcept : handle(other.handle) {
        other.handle = nullptr;
    }

    DefaultCoroutine& operator=(DefaultCoroutine&& other) noexcept {
        if (this not_eq &other) {
            if (handle) {
                handle.destroy(); // ? чаще всего подразумевается что корутина в которую перемещают (корутину которая была возвращена из функции) уже выполнена/не стартовала
            }
            handle = other.handle;
            other.handle = nullptr; // ? для корректного вызова деструктора
        }
        return *this;
    }

private:
    std::coroutine_handle<promise_type> handle;
};

#endif // DefaultCoroutine_H