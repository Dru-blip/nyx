#include "FieldMap.h"


namespace Nyx {
    uint32_t FieldMap::add_entry(const Value &key, const Value &value) {
        uint32_t index = m_values.size();
        m_values.push_back(value);
        m_field_indices[key] = index;
        return index;
    }

    int32_t FieldMap::get_index(const Value &key) const {
        auto it = m_field_indices.find(key);
        if (it != m_field_indices.end()) {
            return it->second;
        }
        return -1;
    }
} // namespace Nyx
