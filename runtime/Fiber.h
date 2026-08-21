#pragma once

#include <cstdint>
#include <vector>
#include "bytecode/Executable.h"
#include "runtime/CallStack.h"
#include "runtime/FrameChunk.h"
#include "runtime/Value.h"


namespace Nyx {
    using FiberId = uint16_t;
    struct Fiber {

        inline Frame *push_frame(bytecode::Executable *executable) {
            return m_stack.push(executable);
        }

        FiberId m_id{};
        CallStack m_stack;
    };
} // namespace Nyx
