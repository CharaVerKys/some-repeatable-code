#pragma once

#include "impl.hpp"
#include <filesystem>

// ! no need to include for log, include only for init and exit

class Logger{
    Logger(){}
    log_impl impl;
    std::filesystem::path logDir = "./";
public:
    inline static Logger* instance(){
        static Logger singleton;
        return &singleton;
    } 
    void setLogDir(std::filesystem::path&& p){
        logDir = std::move(p);
    }
    void init(){
        cvk::flat_map<log::to, std::shared_ptr<std::ofstream>> streams;
        std::string time = __getStringFromCurrentTime(false);
        streams.insert(log::to::main, std::make_shared<std::ofstream>(logDir / (time+"_main.log"),std::ios::app));
        impl.start(streams);
    }
    inline void __push(const log::to &target, const std::string &string){
        impl.push(target, string);
    }
    void exit(){
        impl.stop();
        if(impl.queue.empty()){
            return;
        }
        std::ofstream exitStream(logDir / (__getStringFromCurrentTime(false) + "_exit.log"));
        impl.remainsOnExit(exitStream);
    }
    inline static std::string __getStringFromCurrentTime(bool forLog) {
        auto now = std::chrono::system_clock::now();
        std::time_t now_c = std::chrono::system_clock::to_time_t(now);
        const std::tm *tm = std::localtime(&now_c);
        std::ostringstream oss;
        if(forLog){
            oss << std::setw(2) << std::setfill('0') << tm->tm_hour << ":"
                << std::setw(2) << std::setfill('0') << tm->tm_min  << ":"
                << std::setw(2) << std::setfill('0') << tm->tm_sec;
        }else{
            oss << std::setw(2) << std::setfill('0') << tm->tm_hour << "-"
                << std::setw(2) << std::setfill('0') << tm->tm_mday << "-"
                << std::setw(2) << std::setfill('0') << tm->tm_mon+1;
        }
        return oss.str();
    }
};
