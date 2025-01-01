#include "somemacroses.hpp"
#include <iostream>
#include <optional>

template<typename T>
constexpr bool foo(T t){
    return static_cast<bool>(t);
}

static_assert(foo(1));

isConstexpr(foo(1));
isConstexpr(foo(std::optional<int>()));

template<typename T>
constexpr bool foo2(T t){
    std::cout << std::endl;
    return true;
}

template<typename T>
[[nodiscard]] constexpr bool foo3(T t){
    return true;
}

isConstexpr(foo2(std::optional<int>()));
isConstexpr(foo3(1));
isConstexpr_no_discard(foo3(1));

int main(){
    foo2(std::optional<int>());
    return 1;
}
