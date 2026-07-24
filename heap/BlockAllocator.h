#pragma once

#include <queue>
#include "Block.h"
#include "heap/RegionAllocator.h"

namespace Nyx {
    class BlockAllocator {

    public:
        BlockAllocator(RegionAllocator &region_allocator) : m_region_allocator(region_allocator) {};
        Block *acquire_block();


    private:
        void grow();

        std::queue<Block *> m_free_list;
        RegionAllocator &m_region_allocator;
    };
} // namespace Nyx
