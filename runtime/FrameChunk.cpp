#include "runtime/FrameChunk.h"
#include "mimalloc.h"
#include "runtime/CallStack.h"
#include "runtime/Value.h"


namespace Nyx {
    FrameChunk *FrameChunk::create() {
        void *mem = mi_malloc(FrameChunk::Size);
        FrameChunk *newChunk = new (mem) FrameChunk(mem);
        return newChunk;
    }

    FrameChunk::FrameChunk(void *mem) {
        // point to the start of data area beyond the FrameChunk metadata.
        m_data = static_cast<uint8_t *>(mem) + sizeof(FrameChunk);
    }

    FrameChunk::~FrameChunk() { mi_free(this); }

    Frame *FrameChunk::new_frame(size_t size) {
        uint8_t *mem = m_data + m_next_offset;
        Frame *frame = new (mem) Frame(nullptr);
        m_next_offset += size;
        return frame;
    }

    bool FrameChunk::has_free_space(size_t size) {
        const size_t end = FrameChunk::Size - sizeof(FrameChunk);
        return m_next_offset + size <= end;
    }

    void FrameChunk::acquire_frame(Frame *frame) {
        m_next_offset -= sizeof(Frame) + (frame->m_executable->register_count() * sizeof(Value));
    }
} // namespace Nyx
