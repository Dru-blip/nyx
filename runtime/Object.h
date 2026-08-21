#pragma once


#include "heap/Cell.h"

namespace Nyx {
    class Object : public Cell {
    public:
        virtual bool is_string() const { return false; }
        virtual bool is_native_function() const { return false; }
    };
} // namespace Nyx
