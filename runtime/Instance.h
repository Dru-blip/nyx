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

    protected:
        std::unique_ptr<FieldMap> m_field_map;
    };
} // namespace Nyx
