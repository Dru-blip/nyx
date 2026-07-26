#pragma once

#include <cstdint>
#include <vector>
#include "runtime/CallStack.h"
#include "runtime/FrameChunk.h"
#include "runtime/Value.h"


namespace Nyx {
    using FiberId = uint16_t;
    class Fiber {
    public:
    private:
        FiberId m_id{};
        CallStack m_stack;
        std::vector<Value> registers;
    };
} // namespace Nyx
