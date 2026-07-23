#pragma once

#include <cstdint>


namespace Nyx {
    struct FreeCell {
        FreeCell *next{nullptr};
    };

    class Region;

    class Block {
    public:
        static constexpr size_t Size = 16 * 1024;

        void set_region(Region *region) { m_region = region; }
        FreeCell *get_free_cell();
        bool has_free_cells() const { return m_free_list != nullptr; }

        void format(size_t c_size);

    private:
        FreeCell *m_free_list{nullptr};
        uint8_t *m_data{nullptr};
        size_t cell_size = {0};
        size_t cell_count{0};
        Region *m_region{nullptr};
    };
} // namespace Nyx
