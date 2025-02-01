// #pragma once

// ! include only in logger.hpp

#include "logenum.hpp"
#include <cassert>
#include <condition_variable>
#include <fstream>
#include <functional>
#include <iostream>
#include <queue>
#include <thread>
#include <flat_map.hpp>

struct log_impl{
    std::queue<std::pair<log::to,std::string>> queue;
    std::jthread thread;
    std::mutex mutex;
    std::condition_variable cond_var;
    void work(std::stop_token token, cvk::flat_map<log::to,std::shared_ptr<std::ofstream>> streams){
        while(not token.stop_requested()){
            std::unique_lock lock(mutex);
            cond_var.wait(lock, [this, &token](){//notify only on new task in queue
                return not queue.empty()
                    or token.stop_requested();
            });
            while(not queue.empty()){
                const auto& pair = queue.front();
                                // ? probably main log (should be first)
                *streams.at_tryFirst(1, pair.first) << pair.second;
                //todo check invalid write (errno)
                queue.pop();
            }
        }
        std::cout << "end log" <<std::endl;
    }
    void start(const cvk::flat_map<log::to,std::shared_ptr<std::ofstream>>& streams){
        auto withThis = std::bind_front(&log_impl::work, this);
        thread = std::jthread(withThis, streams);
    }
    void stop(){
        thread.request_stop();
        cond_var.notify_one();
        thread.join();
    }
    void push(const log::to& target, const std::string& string){
        std::unique_lock lock(mutex);
        queue.push({target,string});
        cond_var.notify_one();
    }
    void remainsOnExit(std::ofstream& stream){
        while(not queue.empty()){
            const auto& pair = queue.front();
            stream << "\033[37mlog::to id ( " << (int)pair.first << ")" << pair.second;
            queue.pop();
        }
    }
};