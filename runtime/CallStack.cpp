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


    CallStack::~CallStack() {
        free_cache_chunks();
        while (m_head_chunk) {
            FrameChunk *next = m_head_chunk->m_prev;
            m_head_chunk->~FrameChunk();
            m_head_chunk = next;
        }
    }

    void CallStack::pop() {
        // TODO: should delete the chunk if its empty or cache it.
        assert(m_top != nullptr);

        m_head_chunk->acquire_frame(m_top);
        m_top = m_top->m_prev;


        if (m_head_chunk->m_frame_count == 0) {
            if (m_cached_chunk_count > CallStack::ChunkCacheSize) {
                free_cache_chunks();
            }
            m_head_chunk->m_prev = m_cached_chunk;
            m_cached_chunk = m_head_chunk;
            m_cached_chunk_count++;
        }
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


    void CallStack::free_cache_chunks() {
        while (m_cached_chunk) {
            FrameChunk *next = m_cached_chunk->m_prev;
            m_cached_chunk->~FrameChunk();
            m_cached_chunk = next;
            m_cached_chunk_count--;
        }
    }
} // namespace Nyx
