#ifndef __OPERATIONSCON_H
#define __OPERATIONSCON_H

#include <fstream>
#include <vector>
#include <random>
#include "allocation.h"

namespace op
{

struct Operation
{
	uint64_t u, v;
	Operation(uint64_t u, uint64_t v) : u(u), v(v) {}
	Operation() {}
};


		
		Operation * getOperations(std::string inputstr, uint64_t &n, uint64_t &m) //generate Unite Operations
		{
			n = 0; m = 0;
			
			//read Unite Operations from the file
		
			std::vector<Operation> vec;
			vec.clear();
		
			uint64_t u, v;	
            uint64_t i = 0;
            
			while (i < inputstr.size())
            {
                uint64_t j = i;
                std::string fname = "";
                while (inputstr[j] != ';') { fname += inputstr[j]; j++; }
                //i = j + 1;
                
                std::ifstream inf;
            
                inf.open(fname);
                if (!inf) { fprintf(stdout, "%d\n", (int) (j/i)); }
                uint64_t curn = 0;
                i = j + 1;
                while (inf >> u >> v)
                {
                    vec.push_back(Operation(n + u, n + v));
                    if (u > curn) curn = u;
                    if (v > curn) curn = v;
                    m++;
                }
                n += (curn + 1);
                inf.close();
 			}
			
            Operation * o = new Operation[m];
//            Operation ** o = static_cast<Operation**>(memalloc::CallocAligned(m, sizeof
//                                                                             (Operation*), memalloc::kCachePrefetch * 4));
            
  			for (uint64_t i = 0; i < m; i++)
            {
                
//                o[i] = static_cast<Operation*>(memalloc::get<Operation>(memalloc::kCachePrefetch * 4));
//                *o[i] = vec[i];
                o[i] = vec[i];
            }
            return o;
        }
}
#endif


