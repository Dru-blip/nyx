#pragma once

#include <string_view>
#include <unordered_map>
#include "runtime/String.h"


namespace Nyx {
    class StringPool {
    public:
        StringPool(VM &vm) : m_vm(vm) {}
        String *get_string(std::string_view view);
        String *add_string(std::string_view view);

    private:
        std::unordered_map<std::string_view, String *> m_strings;
        VM &m_vm;
    };
} // namespace Nyx
