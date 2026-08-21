#include "String.h"


namespace Nyx {
    String *String::create(Heap *heap, const char *data, std::size_t size) {
        if (size <= SmallStringSize) {
            return SmallString::create(heap, data, size);
        }
        return HeapString::create(heap, data, size);
    }

    SmallString *SmallString::create(Heap *heap, const char *data, std::size_t size) {
        void *slot = heap->allocate_cell(sizeof(SmallString) + size + 1);
        SmallString *str = new (slot) SmallString(Type::SmallString, size, data);
        return str;
    }

    HeapString *HeapString::create(Heap *heap, const char *data, std::size_t size) {
        void *slot = heap->allocate_cell(sizeof(HeapString));
        HeapString *str = new (slot) HeapString(Type::Heap, size, data);
        return str;
    }
} // namespace Nyx
