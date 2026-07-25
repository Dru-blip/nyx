#pragma once


#include <array>
#include <concepts>
#include <cstddef>
#include "heap/Bin.h"
#include "heap/BlockAllocator.h"
#include "heap/Cell.h"
#include "heap/RegionAllocator.h"


namespace Nyx {
    class Heap {
    public:
        Heap();
        void *allocate_cell(size_t size);

        // Check if T inherits from Cell and allocate a cell for its size and construct it with
        // given arguments.
        template<std::derived_from<Cell> T, typename... Args>
        T *allocate(Args &&...args) {
            void *slot = allocate_cell(sizeof(T));
            T *obj = new (slot) T(std::forward<Args>(args)...);
            return obj;
        }

    private:
        RegionAllocator m_region_allocator;
        BlockAllocator m_block_allocator;
        std::array<Bin, Bin::Count> m_bins;
    };
} // namespace Nyx
