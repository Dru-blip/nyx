#pragma once

#include "runtime/VM.h"
#include "runtime/Value.h"


namespace Nyx {
    class Builtins {
    public:
        static Value print_impl(VM &vm, Value *args, uint8_t arg_count);
        static void register_builtins(VM &vm);
    };
} // namespace Nyx
