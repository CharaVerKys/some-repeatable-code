#pragma once

consteval const char* extract_method_name(const char* fullString) { // обязательно compile-time
    const char* literPos = nullptr;
    for (const char* itPos = fullString; *itPos; ++itPos) {
        if(*itPos == 0){throw "error missing : => invalid method name";}
        if (*itPos == ':') { // последнее совпадение найдётся
            literPos = itPos;
        }
    }
    return literPos ? literPos + 1 : fullString;  // если подан по какой-то причине только метод
}

#include <string_view>
consteval int consteval_strcmp(std::string_view str1, std::string_view str2) {
    if (str1.size() != str2.size()) {
        return str1.size() < str2.size() ? -1 : 1;
    }
    for (size_t i = 0; i < str1.size(); ++i) {
        if (str1[i] != str2[i]) {
            return str1[i] < str2[i] ? -1 : 1;
        }
    }
    return 0;
}