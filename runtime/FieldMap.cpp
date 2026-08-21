#include "FieldMap.h"


namespace Nyx {
    uint32_t FieldMap::add_entry(const Value &key, const Value &value) {
        uint32_t index = m_values.size();
        m_values.push_back(value);
        m_field_indices[key] = index;
        return index;
    }
} // namespace Nyx
