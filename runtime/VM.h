#pragma once


#include <string_view>
#include "heap/Heap.h"
#include "runtime/Fiber.h"
#include "runtime/GlobalObject.h"
#include "runtime/NativeFunction.h"
#include "runtime/StringPool.h"
#include "runtime/Value.h"

namespace Nyx {
    class VM {
    public:
        VM();
        ~VM();

        inline Heap *heap() { return m_heap; }
        inline StringPool *string_pool() { return m_string_pool; }
        inline GlobalObject *global_object() { return m_global_object; }

        Value run_executable(bytecode::Executable *executable);

        uint32_t register_builtin_function(std::string_view &name, NativeFunctionPtr func);

    private:
        Heap *m_heap;
        StringPool *m_string_pool;
        Fiber m_fiber{};
        GlobalObject *m_global_object;
    };
} // namespace Nyx
