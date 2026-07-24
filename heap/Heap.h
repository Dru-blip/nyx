#pragma once


#include <array>
#include <cstddef>
#include "heap/Bin.h"
#include "heap/BlockAllocator.h"
#include "heap/RegionAllocator.h"


namespace Nyx {
    class Heap {
    public:
        void *allocate(size_t size);
        static Heap *create();
        Heap();

    private:
        RegionAllocator m_region_allocator;
        BlockAllocator m_block_allocator;
        std::array<Bin, Bin::Count> m_bins;
    };
} // namespace Nyx
