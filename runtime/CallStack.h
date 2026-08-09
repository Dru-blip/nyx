#pragma once

#include "runtime/Frame.h"
#include "runtime/FrameChunk.h"

namespace Nyx {
    class CallStack {
    public:
        static constexpr size_t ChunkCacheSize = 8;
        // release chunks memory using destructor.
        ~CallStack();

        // returns the top frame.
        Frame *top() const { return m_top; }

        // create and push a new frame into the call stack.
        Frame *push(bytecode::Executable *executable);
        void pop();

    private:
        void grow();
        void try_acquire_chunk(size_t frame_size);

        FrameChunk *m_head_chunk = nullptr;
        Frame *m_top = nullptr;

        // unused fields
        FrameChunk *m_cached_chunk = nullptr;
        size_t m_cached_chunk_count = 0;
    };
} // namespace Nyx
