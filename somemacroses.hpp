#pragma once

#define isConstexpr(expr) \
static_assert([]() constexpr->bool{expr; return true;}())

#define isConstexpr_no_discard(expr) \
static_assert([]() constexpr->bool{auto f = expr; return true;}())

#define PRIVATE_CONTEXT(seq) \
    private: \
    seq \
    public: