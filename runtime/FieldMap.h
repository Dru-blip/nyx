#pragma once


#include <cstdint>
#include <unordered_map>
#include <vector>
#include "heap/Cell.h"
#include "runtime/Value.h"


namespace Nyx {
    class FieldMap {
    public:
        uint32_t add_entry(const Value &key, const Value &value);
        inline Value get_value(uint32_t index) const { return m_values[index]; }
        inline std::size_t size() const { return m_values.size(); }

    private:
        std::unordered_map<Value, uint32_t, ValueHash> m_field_indices;
        std::vector<Value> m_values;
    };
} // namespace Nyx
