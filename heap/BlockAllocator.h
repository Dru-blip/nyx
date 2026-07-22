#pragma once

#include "Block.h"
#include "heap/RegionAllocator.h"

namespace Nyx {
    class BlockAllocator {
    public:
        BlockAllocator(RegionAllocator &region_allocator);
        Block &acquire_block();

    private:
        void grow();

        Block &m_free_list;
        RegionAllocator &m_region_allocator;
    };
} // namespace Nyx
