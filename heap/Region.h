#pragma once

#include <array>
#include "Block.h"

#include <cstdint>

// TODO: i dont know how to correctly use this fucking library.
#include <mimalloc.h>


namespace Nyx {
    class Region {
    public:
        static constexpr size_t Size = 2 * 1024 * 1024;
        static constexpr size_t BlockCount = Region::Size / Block::Size;

        Region();

        Block *get_block(size_t index) { return &m_blocks[index]; }
        std::array<Block, BlockCount> &get_blocks() { return m_blocks; }

    private:
        mi_heap_t *m_heap;
        uint8_t *m_data;
        std::array<Block, BlockCount> m_blocks;
    };
} // namespace Nyx
