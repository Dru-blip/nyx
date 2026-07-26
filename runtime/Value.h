#pragma once
#include <cstdint>


namespace Nyx {
    struct Value {
        enum class Type { Nil, Int };
        Type type;
        int64_t i;

        Value(Type type, int64_t i) : type(type), i(i) {}


        int64_t to_int() const { return i; }
        static Value from_int(int64_t i) { return {Type::Int, i}; }
    };

    static inline Value nil() { return {Value::Type::Nil,0}; }
} // namespace Nyx
