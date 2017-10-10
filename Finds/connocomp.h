
#ifndef __CONNOCOMP_H
#define __CONNOCOMP_H


#include <atomic>
#include "node.h"

uint64_t Find(Node * nodes, uint64_t u, const uint64_t &maxCAS)
{
    uint64_t w = nodes[u].p.load(std::memory_order_relaxed);
    while (u != w)
    {
        u = w;
        w = nodes[u].p.load(std::memory_order_relaxed);
    }
    return u;
}


#endif
