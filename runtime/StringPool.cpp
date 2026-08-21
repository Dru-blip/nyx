#include "runtime/StringPool.h"
#include "runtime/String.h"


namespace Nyx {
    String *StringPool::get_string(std::string_view view) {
        auto it = m_strings.find(view);
        if (it != m_strings.end()) {
            return it->second;
        }
        return nullptr;
    }

    String *StringPool::add_string(std::string_view view) {
        String *str = get_string(view);
        if (str != nullptr) {
            return str;
        }
        str = String::create(m_heap, view.data(), view.size());
        m_strings[view] = str;
        return str;
    }
} // namespace Nyx
