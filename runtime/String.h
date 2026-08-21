#pragma once

#include <cstddef>
#include <cstring>
#include <mimalloc.h>

#include "Object.h"
#include "heap/Heap.h"

namespace Nyx {
    class String : public Object {
    public:
        static constexpr std::size_t SmallStringSize = 31;

        enum class Type {
            SmallString,
            Heap,
        };

        virtual bool is_string() const override { return true; }

        static String *create(Heap *heap, const char *data, std::size_t size);

        String(Type type, std::size_t size) : m_type(type), m_size(size) {}

    protected:
        Type m_type;
        std::size_t hash;
        std::size_t m_size;
    };

    class SmallString : public String {
    public:
        static SmallString *create(Heap *heap, const char *data, std::size_t size);

        SmallString(Type type, std::size_t size, const char *data) : String(type, size) {
            char *d = reinterpret_cast<char *>(this + 1);
            std::memcpy(d, data, size);
            d[size] = '\0';
        }
    };

    class HeapString : public String {
    public:
        static HeapString *create(Heap *heap, const char *data, std::size_t size);

        HeapString(Type type, std::size_t size, const char *data) : String(type, size) {
            //TODO: should check if mi_malloc returns null
            char *d = static_cast<char *>(mi_malloc(size + 1));
            std::memcpy(d, data, size);
            d[size] = '\0';
            m_data = d;
        }
        char *m_data;
    };
} // namespace Nyx
