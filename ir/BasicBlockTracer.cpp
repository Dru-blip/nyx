#include "BasicBlockTracer.h"
#include <queue>
#include <vector>
#include "ir/BasicBlock.h"


namespace Nyx::ir {
    std::vector<BasicBlock *> BasicBlockTracer::trace() {
        std::vector<bool> visited(m_blocks.size(), false);
        std::vector<BasicBlock *> ordered;

        while (true) {
            // naively check for unvisited blocks to start the trace.
            BasicBlock *start = nullptr;
            for (auto block: m_blocks) {
                if (!visited[block->id()]) {
                    start = block;
                    break;
                }
            }
            if (!start)
                break;

            std::queue<BasicBlock *> queue;
            queue.push(start);

            while (!queue.empty()) {
                BasicBlock *block = queue.front();
                visited[block->id()] = true;
                queue.pop();
                ordered.push_back(block);

                // TODO: should select a successor based on some heuristic.
                // instead of picking first available successor.
                for (auto successor: block->successors()) {
                    if (!visited[successor->id()]) {
                        queue.push(successor);
                    }
                }
            }
        }

        return ordered;
    }
} // namespace Nyx::ir
