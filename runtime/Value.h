#pragma once
#include <cstdint>


namespace Nyx {
    struct Value {
        enum class Type { Int };
        Type type;
        int64_t i;
    };
} // namespace Nyx
