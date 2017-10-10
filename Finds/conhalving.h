
#ifndef __CONHALVING_H
#define __CONHALVING_H

#include <atomic>

uint64_t Find(Node * nodes, uint64_t u, const uint64_t &maxCAS)
{
    while (1)
    {
        uint64_t v = nodes[u].p.load(std::memory_order_relaxed);
        uint64_t w = nodes[v].p.load(std::memory_order_relaxed);
        if (v == w) return v;
        else
        {
            nodes[u].p.compare_exchange_strong(v, w));
            u = nodes[u].p.load(std::memory_order_relaxed);
        }
    }
}

#endif
