#include "Block.h"

namespace Nyx {
    FreeCell *Block::get_free_cell() {
        if (m_free_list) {
            FreeCell *cell = m_free_list;
            m_free_list = cell->next;
            return cell;
        }
        return nullptr;
    }

    void Block::format(size_t c_size) {
        cell_size = c_size;
        cell_count = Size / c_size;

        for (size_t i = 0; i < cell_count; ++i) {
            FreeCell *cell = reinterpret_cast<FreeCell *>(m_data + i * cell_size);
            cell->next = m_free_list;
            m_free_list = cell;
        }
    }
} // namespace Nyx
