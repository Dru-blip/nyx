#include "Builtins.h"
#include <print>
#include "runtime/Value.h"

namespace Nyx {
    Value Builtins::print_impl(VM &vm, Value *args, uint8_t arg_count) {
        (void) vm;
        (void) arg_count;

        const auto arg = args[0];
        std::println("{:d}", arg.as_int());
        return Nil;
    }

    void Builtins::register_builtins(VM &vm) {
        std::string_view name = "print";
        vm.register_builtin_function(name, print_impl);
    }
} // namespace Nyx
