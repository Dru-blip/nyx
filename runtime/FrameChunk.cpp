#include "runtime/FrameChunk.h"
#include "mimalloc.h"
#include "runtime/CallStack.h"


namespace Nyx {
    static constexpr size_t FrameCount = (FrameChunk::Size - sizeof(FrameChunk)) / sizeof(Frame);

    FrameChunk *FrameChunk::create() {
        void *mem = mi_malloc(FrameChunk::Size);
        FrameChunk *newChunk = new (mem) FrameChunk(mem);
        return newChunk;
    }

    FrameChunk::FrameChunk(void *mem) {
        // point to the start of data area beyond the FrameChunk metadata.
        m_data = static_cast<uint8_t *>(mem) + sizeof(FrameChunk);
        for (size_t i = 0; i < FrameCount; i++) {
            Frame *frame = new (m_data + i * sizeof(Frame)) Frame(nullptr);
            frame->m_prev = m_free_frame;
        }
    }

    FrameChunk::~FrameChunk() { mi_free(this); }

    Frame *FrameChunk::new_frame() {
        Frame *frame = m_free_frame;
        m_free_frame = frame->m_prev;
        return frame;
    }

    void FrameChunk::acquire_frame(Frame *frame) {
        frame->m_prev = m_free_frame;
        m_free_frame = frame;
    }
} // namespace Nyx
