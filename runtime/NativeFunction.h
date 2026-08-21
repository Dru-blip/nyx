#pragma once

#include <cstdint>
#include "runtime/Object.h"
#include "runtime/String.h"
#include "runtime/Value.h"

namespace Nyx {
    using NativeFunctionPtr = Value (*)(Value *args, std::uint8_t arg_count);
    class NativeFunction : public Object {

    public:
        static NativeFunction *create(Heap *heap, String *name, NativeFunctionPtr func);
        NativeFunction(String *name, NativeFunctionPtr func) : m_name(name), m_func(func) {}

        inline Value call(Value *args, std::uint8_t arg_count) { return m_func(args, arg_count); }
        String *name() const { return m_name; }

        bool is_native_function() const override { return true; }

    private:
        String *m_name;
        NativeFunctionPtr m_func;
    };
} // namespace Nyx
