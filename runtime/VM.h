#pragma once


#include "heap/Heap.h"
#include "runtime/Fiber.h"
#include "runtime/StringPool.h"
#include "runtime/Value.h"

namespace Nyx {
    class VM {
    public:
        VM();
        ~VM();

        inline Heap *heap() { return m_heap; }

        Value run_executable(bytecode::Executable *executable);

    private:
        Heap *m_heap;
        StringPool *m_string_pool;
        Fiber m_fiber{};
    };
} // namespace Nyx
