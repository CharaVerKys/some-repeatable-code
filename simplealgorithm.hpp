#pragma once

#include <iterator>
#include <optional>
#include "unittype.h"

enum only_one_true_result : std::int8_t{
    nothingFound = -1,
    foundOne = 1,
    foundMoreThatOne = 0
};

namespace cvk::algo{

template<std::forward_iterator It, std::sentinel_for<It> Sent>
[[nodiscard]] only_one_true_result only_one_true(It first, Sent last)
requires requires(It it){{static_cast<bool>(*it)} -> std::same_as<bool>;}
{
    bool everFound = false;
    while(first not_eq last){
        if(static_cast<bool>(*first)){
            if(everFound){
                return foundMoreThatOne;
            }
            everFound = true;
        }
        ++first;
    }
    return everFound ? foundOne : nothingFound;
}
template<std::ranges::forward_range Range>
[[nodiscard]] only_one_true_result only_one_true(Range const& range)
requires requires(Range r){{static_cast<bool>(*r.begin())} -> std::same_as<bool>;}
{
    return only_one_true(std::ranges::begin(range), std::ranges::end(range));
}


}