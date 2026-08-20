#include "BasicBlockTracer.h"
#include <cstdio>
#include <print>
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
                BasicBlock *likely_successor = nullptr;
                size_t likely_weight = 0;
                for (auto &successor: block->successors()) {
                    if (!visited[successor.to->id()]) {
                        if (successor.weight > likely_weight) {
                            likely_successor = successor.to;
                            likely_weight = successor.weight;
                        }
                    }
                }

                if (likely_successor){
                    queue.push(likely_successor);
                }
            }
        }

        return ordered;
    }
} // namespace Nyx::ir
