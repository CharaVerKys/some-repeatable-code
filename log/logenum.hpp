#pragma once
namespace cvk::log{
    enum class to{
        main
    };
    enum class lvl{
        debug,
        good,
        norm,
        error,
        critical
    };
}
// ? if conflict, comment this and define yourself, or use full namespace
using clt = cvk::log::to;
using cll = cvk::log::lvl;
