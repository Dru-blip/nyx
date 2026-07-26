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

        Frame *new_frame();
        bool has_free_frame() const { return m_free_frame != nullptr; }
        void set_previous(FrameChunk *previous) { m_prev = previous; }
        void acquire_frame(Frame *frame);


    private:
        uint8_t *m_data = nullptr;
        Frame *m_free_frame = nullptr;
        FrameChunk *m_prev = nullptr;

        friend class CallStack;
    };
} // namespace Nyx
