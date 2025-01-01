#pragma once
#if __has_include(<experimental/source_location>)
    #include <experimental/source_location>
#else
    #include <source_location>
#endif
#include <QDebug>
#include <cassert>

#if __has_include(<experimental/source_location>)
inline static auto helper = [](const char* method, const std::experimental::source_location& location = std::experimental::source_location::current()){ 
#else
inline static auto helper = [](const char* method, const std::source_location& location = std::source_location::current()){ 
#endif
        qCritical()<< "failed to run function " << method << " in INVOKE on location: " <<location.file_name() << " line: " << location.line();
        assert(false&&"invoke macros");
        return false;
    };

#define INVOKE(context, method_ptr) \
    QMetaObject::invokeMethod(context, extract_method_name(#method_ptr), Qt::QueuedConnection) ? true \
    : helper(#method_ptr)

#define INVOKE_ARGS(context, method_ptr, ...) \
    QMetaObject::invokeMethod(context, extract_method_name(#method_ptr), Qt::QueuedConnection, __VA_ARGS__) ? true \
    : helper(#method_ptr)

#define INVOKE_DIRECT(context, method_ptr) \
    QMetaObject::invokeMethod(context, extract_method_name(#method_ptr), Qt::DirectConnection) ? true \
    : helper(#method_ptr)

#define INVOKE_ARGS_DIRECT(context, method_ptr, ...) \
    QMetaObject::invokeMethod(context, extract_method_name(#method_ptr), Qt::DirectConnection, __VA_ARGS__) ? true \
    : helper(#method_ptr)

#define INVOKE_BLOCKED(context, method_ptr) \
    QMetaObject::invokeMethod(context, extract_method_name(#method_ptr), Qt::BlockingQueuedConnection) ? true \
    : helper(#method_ptr)

#define INVOKE_ARGS_BLOCKED(context, method_ptr, ...) \
    QMetaObject::invokeMethod(context, extract_method_name(#method_ptr), Qt::BlockingQueuedConnection, __VA_ARGS__) ? true \
    : helper(#method_ptr)

// ? usage: INVOKE(qobject, &Class::method);
// ? usage: INVOKE(qobject, Class::method);
// ? usage: INVOKE(qobject, ::method);

// ? usage: INVOKE_ARGS(qobject, &Class::method, Q_ARG(type,value),... );