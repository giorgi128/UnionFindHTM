
#ifndef __CONSPLITTING_H
#define __CONSPLITTING_H

#include <atomic>

uint64_t Find(Node * nodes, uint64_t u, const uint64_t &maxCAS)
{
    uint64_v, w;
    while (1)
    {
        temp=u;
        for (int i=0; i<2;i++)
        {
            v = nodes[u].p.load(std::memory_order_relaxed);
            w = nodes[v].p.load(std::memory_order_relaxed);
            if (v == w) return v;
            else
            {
                uint64_t temp = v; //use temp because value of v can be changed in the next command
                nodes[u].p.compare_exchange_strong(v, w);
            }
        }
        u = temp;
    }
}

#endif
