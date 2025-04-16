#include <log/logger.hpp> // need only for init and exit
#include <log/write> // for logging

using namespace std::chrono_literals;
int main(){
    std::filesystem::create_directory("log");
    Logger::instance()->setLogDir("log");
    Logger::instance()->init();
    cvk::write(log::to::main) << log::lvl::good << "log start";
    cvk::write(log::to::main) << "norm";
    cvk::write(log::to::main) << log::lvl::critical << "crit";

    using namespace log;
    using write_ = cvk::write;
    write_(to::main) << lvl::good << "less code";
    #define good_log write_(to::main) << lvl::good
    good_log << "macros...";

    Logger::instance()->exit();
    std::exit(0);
}