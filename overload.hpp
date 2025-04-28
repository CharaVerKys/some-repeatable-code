#pragma once

namespace cvk{
template <class ...Functor>
struct overload : Functor...{using Functor::operator()...;};
// template <class ...Functor>
// overload(Functor...) -> overload<Functor...>;
}
