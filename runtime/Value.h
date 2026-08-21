#pragma once
#include <cassert>
#include <cstdint>
#include <print>
#include <sys/types.h>
#include "runtime/Object.h"


namespace Nyx {
    class Value {
    public:
        static constexpr uintptr_t Mask = 0x7;

        static constexpr uintptr_t ObjTag = 0ull;
        static constexpr uintptr_t IntTag = 1ull;
        static constexpr uintptr_t NilTag = 2ull;
        static constexpr uintptr_t BoolTag = 3ull;

        Value() = default;
        explicit Value(int64_t value) { m_raw = static_cast<uintptr_t>(value) << 3 | IntTag; }

        static Value from_raw(uintptr_t raw) {
            Value value;
            value.m_raw = raw;
            return value;
        }

        static Value from_bool(bool value) {
            return Value::from_raw(static_cast<uintptr_t>(value) << 3 | BoolTag);
        }

        static Value from_object(Object *obj) {
            const uintptr_t raw = reinterpret_cast<uintptr_t>(obj);
            // check for pointer alignment to 8 bytes.
            assert((raw & Mask) == 0);
            return Value::from_raw(raw);
        }

        bool is_int() const { return (m_raw & Mask) == IntTag; }
        bool is_obj() const { return (m_raw & Mask) == ObjTag; }
        bool is_nil() const { return (m_raw & Mask) == NilTag; }
        bool is_bool() const { return (m_raw & Mask) == BoolTag; }
        bool is_false() const { return is_bool() && !as_bool(); }
        bool is_truthy() const { return !is_nil() && !is_false(); }

        int64_t as_int() const { return static_cast<int64_t>(m_raw) >> 3; }
        bool as_bool() const { return static_cast<bool>(m_raw >> 3); }
        Object *as_obj() const { return reinterpret_cast<Object *>(m_raw); }
        uintptr_t raw() const { return m_raw; }

        void print_tag() const {
            switch (m_raw & Mask) {
                case IntTag:
                    std::println("IntTag");
                    break;
                case ObjTag:
                    std::println("ObjTag");
                    break;
                case NilTag:
                    std::println("NilTag");
                    break;
                case BoolTag:
                    std::println("BoolTag");
                    break;
            }
        }

    private:
        uintptr_t m_raw{0};
    };

    static Value Nil = Value::from_raw(Value::NilTag);
    static Value True = Value::from_bool(true);
    static Value False = Value::from_bool(false);
} // namespace Nyx
