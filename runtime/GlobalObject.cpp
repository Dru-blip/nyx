#include "runtime/GlobalObject.h"


namespace Nyx {
    GlobalObject *GlobalObject::create(Heap *heap) {
        void *slot = heap->allocate_cell(sizeof(GlobalObject));
        return new (slot) GlobalObject();
    }

    GlobalObject::GlobalObject() { m_field_map = std::make_unique<FieldMap>(); }
} // namespace Nyx
