
#ifndef __SEQSPLITTING_H
#define __SEQSPLITTING_H

uint64_t Find(uint64_t *p, uint64_t u)
{
    while (1)
    {
        uint64_t v = p[u];
        uint64_t w = p[v];
        if (v == w) return v;
        else
        {
            p[u] = w;
            u = v;
        }
    }
}


#endif
