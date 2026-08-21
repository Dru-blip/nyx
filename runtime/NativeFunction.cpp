#include "NativeFunction.h"


namespace Nyx {
    NativeFunction *NativeFunction::create(Heap *heap, String *name, NativeFunctionPtr func) {
        void *slot = heap->allocate_cell(sizeof(NativeFunction));
        return new (slot) NativeFunction(name, func);
    }
} // namespace Nyx
