#pragma once
#include <cstdint>


namespace Nyx {
    class Value {
    public:
        static constexpr uintptr_t Mask = 0x7;

        static constexpr uintptr_t IntTag = 1ull;
        static constexpr uintptr_t ObjTag = 0ull;
        static constexpr uintptr_t NilTag = 2ull;

        Value() = default;
        explicit Value(int64_t value) { m_raw = static_cast<uintptr_t>(value) << 3 | IntTag; }
        static Value from_raw(uintptr_t raw) {
            Value value;
            value.m_raw = raw;
            return value;
        }


        bool is_int() const { return (m_raw & Mask) == IntTag; }
        bool is_obj() const { return (m_raw & Mask) == ObjTag; }
        bool is_nil() const { return (m_raw & Mask) == NilTag; }

        int64_t as_int() const { return static_cast<int64_t>(m_raw) >> 3; }
        uintptr_t raw() const { return m_raw; }

    private:
        uintptr_t m_raw{0};
    };

    static Value Nil = Value::from_raw(Value::NilTag);
} // namespace Nyx
