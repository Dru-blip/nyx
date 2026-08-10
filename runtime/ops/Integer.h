#pragma once

#include "runtime/Fiber.h"
#include "runtime/Value.h"


namespace Nyx::Ops::Integer {
    static inline Value add(const Value a, const Value b) {
        // TODO: handle overflow
        return Value(a.as_int() + b.as_int());
    }

    static inline Value sub(const Value a, const Value b) {
        // TODO: handle overflow
        return Value(a.as_int() - b.as_int());
    }

    static inline Value mul(const Value a, const Value b) {
        // TODO: handle overflow
        return Value(a.as_int() * b.as_int());
    }

    static inline Value div(const Value a, const Value b) {
        // TODO: handle overflow
        // TODO: handle division by zero
        return Value(a.as_int() / b.as_int());
    }
} // namespace Nyx::Ops::Integer
