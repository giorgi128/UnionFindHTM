
#ifndef __CONNOCOMP_H
#define __CONNOCOMP_H


#include <atomic>

uint64_t Find(std::atomic<uint64_t> **p, uint64_t u, const uint64_t &maxCAS)
{
    uint64_t w = p[u]->load(std::memory_order_relaxed);
    while (u != w)
    {
        u = w;
        w = p[u]->load(std::memory_order_relaxed);
    }
    return u;
}


#endif
