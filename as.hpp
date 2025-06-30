// make it work
// copied code, not my original, just this code sooo nice i want it to have here

#include <cstddef>
#include <new>
#include <utility>
#include <variant>

template <typename T>
struct AsExpr
{
    T value;

    template <typename U>
    constexpr auto operator->*(U *)
    {
        return std::get_if<U>(&value);
    }
};

struct AsFactory
{
    template <typename T>
    friend auto operator->*(T &&other, AsFactory) -> AsExpr<T &&>
    {
        return {.value = std::forward<T>(other)};
    }
};

struct DummyNew {explicit DummyNew() = default;};
constexpr void *operator new(std::size_t, DummyNew) noexcept {return nullptr;}

#define as ->* AsFactory{} ->* new(DummyNew{})


#include <iostream>
#include <variant>

int main()
{
    std::variant<int, float> v = 1.2f;

    if (auto i = v as int)
        std::cout << *i << '\n';
    else if (auto f = v as float)
        std::cout << *f << '\n';
}


#include <cstddef>
#include <xieite/pp/arrow.hpp>
#include <xieite/pp/fwd.hpp>

#define XIEITE_AS ->*DETAIL_XIEITE::AS::impl()->*::new(DETAIL_XIEITE::AS::dummy())

namespace DETAIL_XIEITE::AS {
    template<typename T>
    struct proxy {
        T&& lhs;

        template<typename U>
        [[nodiscard]] constexpr auto operator->*(U*)
            XIEITE_ARROW(static_cast<U>(XIEITE_FWD(this->lhs)))
    };

    struct impl {
        friend constexpr auto operator->*(auto&& lhs, DETAIL_XIEITE::AS::impl) noexcept {
            return DETAIL_XIEITE::AS::proxy(XIEITE_FWD(lhs));
        }
    };

    struct dummy {
        explicit dummy() = default;
    };
}

constexpr void* operator new(std::size_t, DETAIL_XIEITE::AS::dummy) noexcept {
    return nullptr;
}

static_assert(5.5 XIEITE_AS int == 5);
static_assert(noexcept(5.5 XIEITE_AS int));

int main() {}


