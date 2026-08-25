#pragma once

#include <string_view>
#include "heap/Heap.h"
#include "runtime/FieldMap.h"
#include "runtime/NativeFunction.h"
#include "runtime/Object.h"


namespace Nyx {
    class VM;
    class Instance : public Object {
    public:
        static Instance *create(Heap *heap);
        Instance();

        uint32_t put_field(const Value &key, const Value &value);
        uint32_t put_native_function(VM &vm, String *name, NativeFunctionPtr func);

        int32_t get_field(const Value &key) const;

        inline Value get_field_value(int32_t index) const { return m_field_map->get_value(index); }

        Value get_field_by_key(const Value &key) const {
            int32_t index = m_field_map->get_index(key);
            return m_field_map->get_value(index);
        }

    protected:
        std::unique_ptr<FieldMap> m_field_map;
    };
} // namespace Nyx
