#pragma once

#include <iostream>
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

    static inline Value lt(const Value a, const Value b) { return Value::from_bool(a.as_int() < b.as_int()); }


    static inline Value lte(const Value a, const Value b) {
        return Value::from_bool(a.as_int() <= b.as_int());
    }

    static inline Value gt(const Value a, const Value b) {
        return Value::from_bool(static_cast<bool>(a.as_int() > b.as_int()));
    }

    static inline Value gte(const Value a, const Value b) {
        return Value::from_bool(a.as_int() >= b.as_int());
    }
    static inline Value eq(const Value a, const Value b) { return Value::from_bool(a.as_int() == b.as_int()); }

    static inline Value neq(const Value a, const Value b) {
        return Value::from_bool(a.as_int() != b.as_int());
    }

    static inline Value neg(const Value a) { return Value(-a.as_int()); }

    static inline Value not_(const Value a) { return Value(!a.as_int()); }
} // namespace Nyx::Ops::Integer
