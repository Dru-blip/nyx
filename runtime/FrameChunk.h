#pragma once

#include <cstdint>

#include "runtime/Frame.h"


namespace Nyx {
    class FrameChunk {
    public:
        static constexpr size_t Size = 4 * 1024;
        static FrameChunk *create();
        FrameChunk(void *mem);
        ~FrameChunk();

        Frame *new_frame(size_t size);
        void set_previous(FrameChunk *previous) { m_prev = previous; }
        void acquire_frame(Frame *frame);
        bool has_free_space(size_t size);

    private:
        uint8_t *m_data = nullptr;
        FrameChunk *m_prev = nullptr;
        size_t m_frame_count = 0;
        size_t m_next_offset = 0;

        friend class CallStack;
    };
} // namespace Nyx
