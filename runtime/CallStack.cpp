#include "CallStack.h"
#include <cassert>
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

    CallStack::~CallStack() {
        while (m_head_chunk) {
            FrameChunk *next = m_head_chunk->m_prev;
            delete m_head_chunk;
            m_head_chunk = next;
        }
    }

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
        FrameChunk *chunk = FrameChunk::create();
        chunk->set_previous(m_head_chunk);
        m_head_chunk = chunk;
    }
} // namespace Nyx
