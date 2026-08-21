#include "NativeFunction.h"
#include "runtime/VM.h"


namespace Nyx {
    NativeFunction *NativeFunction::create(VM &vm, String *name, NativeFunctionPtr func) {
        void *slot = vm.heap()->allocate_cell(sizeof(NativeFunction));
        return new (slot) NativeFunction(name, func);
    }
} // namespace Nyx
