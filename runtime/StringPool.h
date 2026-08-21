#pragma once

#include <memory>
#include <string_view>
#include <unordered_map>
#include "runtime/String.h"


namespace Nyx {
    class StringPool {
    public:
        StringPool(std::shared_ptr<Heap> heap) : m_heap(heap) {}
        String *get_string(std::string_view view);
        String *add_string(std::string_view view);

    private:
        std::unordered_map<std::string_view, String *> m_strings;
        std::shared_ptr<Heap> m_heap;
    };
} // namespace Nyx
