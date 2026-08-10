#include "CallStack.h"
#include <cassert>
#include <mimalloc.h>
#include "runtime/FrameChunk.h"


namespace Nyx {
    Frame *CallStack::push(bytecode::Executable *executable) {
        const size_t frame_size = sizeof(Frame) + executable->register_count() * sizeof(Value);
        try_acquire_chunk(frame_size);
        Frame *frame = m_head_chunk->new_frame(frame_size);
        frame->m_executable = executable;
        frame->m_prev = m_top;
        m_top = frame;
        return frame;
    }

    CallStack::CallStack() { m_heap = mi_heap_new(); }

    CallStack::~CallStack() { mi_heap_destroy(m_heap); }

    void CallStack::pop() {
        assert(m_top != nullptr);

        m_top = m_top->m_prev;
        m_head_chunk->acquire_frame(m_top);

        // TODO: should delete the chunk if its empty or cache it.
        // if (m_top == nullptr) {

        // }
    }

    void CallStack::try_acquire_chunk(size_t frame_size) {
        if (m_head_chunk && m_head_chunk->has_free_space(frame_size)) {
            return;
        }

        grow();
    }

    void CallStack::grow() {
        void* mem = mi_heap_malloc(m_heap, FrameChunk::Size);
        FrameChunk *chunk = new (mem) FrameChunk(mem);
        chunk->set_previous(m_head_chunk);
        m_head_chunk = chunk;
    }
} // namespace Nyx
