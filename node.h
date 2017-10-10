/* 
 * File:   node.h
 * Author: trbot
 *
 * Created on September 8, 2017, 9:03 PM
 */

#ifndef NODE_H
#define	NODE_H

#include <atomic>

class Node {
public:
    std::atomic<uint64_t> p;
    std::atomic<uint64_t> id;
    Node() {}
};

#endif	/* NODE_H */

