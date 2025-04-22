#define call_(obj, method) \
    [&obj](auto&& expect) { \
        return std::invoke(&std::remove_pointer_t<std::remove_reference_t<decltype(obj)>>::method, obj, std::forward<decltype(expect)>(expect)); \
    }
#define call_this(method) \
    [this](auto&& expect) { \
        return std::invoke(&std::remove_pointer_t<decltype(this)>::method, this, std::forward<decltype(expect)>(expect)); \
    }