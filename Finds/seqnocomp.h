
#ifndef __SEQNOCOMP_H
#define __SEQNOCOMP_H

uint64_t Find(uint64_t *p, uint64_t u)
{
    while (u != p[u]) u = p[u];
    return u;
}

#endif
