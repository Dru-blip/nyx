#include <memory>
#include "heap/Heap.h"
#include "runtime/FieldMap.h"
#include "runtime/Instance.h"
#include "runtime/Object.h"


namespace Nyx {
    class GlobalObject : public Instance {
    public:
        static GlobalObject *create(Heap *heap);

        GlobalObject();
    };
} // namespace Nyx
