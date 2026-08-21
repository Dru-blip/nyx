#include "String.h"
#include "runtime/VM.h"

namespace Nyx {
    String *String::create(VM &vm, const char *data, std::size_t size) {
        if (size <= SmallStringSize) {
            return SmallString::create(vm.heap(), data, size);
        }
        return HeapString::create(vm.heap(), data, size);
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

    std::string_view String::as_view() const {
        if (m_type == Type::SmallString) {
            const SmallString *small = reinterpret_cast<const SmallString *>(this);
            return std::string_view(small->data(), small->m_size);
        }

        const HeapString *heap = reinterpret_cast<const HeapString *>(this);
        return std::string_view(heap->m_data, heap->m_size);
    }
} // namespace Nyx
