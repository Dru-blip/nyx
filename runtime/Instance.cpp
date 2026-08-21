#include "runtime/Instance.h"
#include "runtime/NativeFunction.h"
#include "runtime/Value.h"

#include "runtime/VM.h"

namespace Nyx {
    Instance *Instance::create(Heap *heap) {
        void *slot = heap->allocate_cell(sizeof(Instance));
        return new (slot) Instance();
    }

    Instance::Instance() { m_field_map = std::make_unique<FieldMap>(); }

    uint32_t Instance::put_field(const Value &key, const Value &value) {
        return m_field_map->add_entry(key, value);
    }

    uint32_t Instance::put_native_function(VM &vm, String *name,
                                           NativeFunctionPtr func) {
        NativeFunction *native_func = NativeFunction::create(vm, name, func);

        return m_field_map->add_entry(Value::from_object(name), Value::from_object(native_func));
    }
} // namespace Nyx
