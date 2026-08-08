#pragma once
#include <cstdint>
#include <optional>


namespace Nyx {
    class Value {
    public:
        static constexpr uintptr_t Mask = 0x7;

        static constexpr uintptr_t IntTag = 1ull;
        static constexpr uintptr_t ObjTag = 0ull;
        static constexpr uintptr_t NilTag = 2ull;


        bool is_int() const { return (m_raw & Mask) == IntTag; }
        bool is_obj() const { return (m_raw & Mask) == ObjTag; }
        bool is_nil() const { return (m_raw & Mask) == NilTag; }

        Value() = default;
        Value(int64_t value) { m_raw = value << 3 | IntTag; }
        Value(uintptr_t tag) { m_raw = tag; }

        uintptr_t raw() const { return m_raw; }

    private:
        uintptr_t m_raw{0};
    };

    static Value Nil = Value(Value::NilTag);
} // namespace Nyx
