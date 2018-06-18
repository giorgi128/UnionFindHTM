#include <iostream>
#include <random>
#include <stdio.h>
#include <thread>
#include <vector>
#include <ctime>
#include <atomic>
#include <fstream>
#include "operations.h"

#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#endif

#ifndef STR
    #define STR(x) XSTR(x)
    #define XSTR(x) #x
#endif
#ifdef FIND_H_FILE
    #include STR(FIND_H_FILE)
#else
    #warning "compiling using findversion.h"
    #include "findversion.h"
#endif

#include "papi_util_impl.h"
__thread int tid = 0;

#define __HANDLE_STATS(handle_stat) \
    handle_stat(LONG_LONG, op_unite, 1, { \
            stat_output_item(PRINT_RAW, SUM, TOTAL) \
    }) \
    handle_stat(LONG_LONG, op_sameset, 1, { \
            stat_output_item(PRINT_RAW, SUM, TOTAL) \
    })

#include "stats_global.h"
GSTATS_DECLARE_STATS_OBJECT(MAX_TID_POW2);
GSTATS_DECLARE_ALL_STAT_IDS;

volatile char padding0[192];
uint64_t *p;
uint64_t n;
uint64_t* rank;
int seed;

bool SameSet(uint64_t u, uint64_t v)
{
    u = Find(p, u);
    v = Find(p, v);
    return (u == v);
}

void Destr()
{
    delete [] p;
    delete [] rank;
}

void Comp(uint64_t *res) //compute array of minimum index nodes in the components
{
    for (uint64_t i = 0; i < n; i++) res[i] = i;
    for (uint64_t i = 0; i < n; i++)
    {
        uint64_t j = i;
        while (p[j] != j) j = p[j];
        if (res[j] > i) res[j] = i;
    }
    for (uint64_t i = 0; i < n; i++)
    {
        uint64_t j = i;
        while (p[j] != j) j = p[j];
        res[i] = res[j];
    }
}


void Init(int seed)
{
    p = new uint64_t[n];
    rank = new uint64_t[n];
    for (uint64_t i = 0; i < n; i++) p[i] = i;
 
    std::mt19937 rng;
    rng.seed(seed);
    //create a random permutation
    for (uint64_t i = 0;  i < n; i++) rank[i] = i;
    for (uint64_t i = 0; i < n; i++)
    {
        std::uniform_int_distribution<uint64_t> rnd_st(i, n - 1);
        uint64_t j = rnd_st(rng);
        uint64_t temp = rank[i];
        rank[i] = rank[j];
        rank[j] = temp;
        
    }
}

bool Unite(uint64_t u, uint64_t v)
{
    while (1)
    {
        u = Find(p, u);
        v = Find(p, v);
        if (u == v) return 1;
        if (rank[u] < rank[v])
        {
            p[u] = v;
            return 0;
        }
        else
        {
            p[v] = u;
            return 0;
        }
    }
}


double
timediff_in_s(const struct timespec &start,
              const struct timespec &end)
{
    struct timespec tmp;
    if (end.tv_nsec < start.tv_nsec) {
        tmp.tv_sec = end.tv_sec - start.tv_sec - 1;
        tmp.tv_nsec = 1000000000 + end.tv_nsec - start.tv_nsec;
    } else {
        tmp.tv_sec = end.tv_sec - start.tv_sec;
        tmp.tv_nsec = end.tv_nsec - start.tv_nsec;
    }
    
    return tmp.tv_sec + (double)tmp.tv_nsec / 1000000000.0;
}


void get_monotonic_time(struct timespec *curtime)
{
#ifdef __MACH__
    clock_serv_t cclock;
    mach_timespec_t mts;
    host_get_clock_service(mach_host_self(), SYSTEM_CLOCK, &cclock);
    clock_get_time(cclock, &mts);
    mach_port_deallocate(mach_task_self(), cclock);
    curtime->tv_sec = mts.tv_sec;
    curtime->tv_nsec = mts.tv_nsec;
#else
    clock_gettime(CLOCK_MONOTONIC, curtime);
#endif
}

//run sequential and measure time


std::pair<double,double> runSequential(uint64_t n, op::Operation* aUnite, uint64_t mUnite,
                     uint64_t mSameSet, uint64_t* resComponent, int seed)
{
    
    Init(seed);
    
    papi_create_eventset(tid);
    
    struct timespec start1, end1, start2, end2;
    
    std::mt19937 rng;
    rng.seed(seed);
    
    std::uniform_int_distribution<uint64_t> rnd_st(0, n - 1);
    
    
    get_monotonic_time(&start1);
    //clock_gettime(CLOCK_MONOTONIC, &start1);
    papi_start_counters(tid);
    
    for (uint64_t i = 0; i < mUnite; i++)
    {
        Unite(aUnite[i].u, aUnite[i].v);
        GSTATS_ADD(tid, op_unite, 1);
    }
    
    //clock_gettime(CLOCK_MONOTONIC, &end1);
    get_monotonic_time(&end1);
    papi_stop_counters(tid);
    
    Comp(resComponent);
    
    //clock_gettime(CLOCK_MONOTONIC, &start2);
    get_monotonic_time(&start2);
    
    for (uint64_t i = 0; i < mSameSet; i++)
    {
        uint64_t u, v;
        u = rnd_st(rng);
        v = rnd_st(rng);
        SameSet(u, v);
        GSTATS_ADD(tid, op_sameset, 1);
    }
    
    //clock_gettime(CLOCK_MONOTONIC, &end2);
    get_monotonic_time(&end2);
    
    Destr();
    
    return std::pair<double, double>(timediff_in_s(start1, end1), timediff_in_s(start2, end2));
}


int main()
{
    uint64_t mSameSet, mUnite, mExtraUnite, nExtraUnite;
    std::string inputstr;
    
    inputstr="roadNet-CA.txt;";
    
    std::cin >> mSameSet >> seed;

    GSTATS_CREATE_ALL;
    GSTATS_CLEAR_ALL;
    papi_init_program(MAX_TID_POW2);
    
    op::Operation *aUnite = op::getOperations(inputstr, n, mUnite);
    
    
    uint64_t* resComponent = new uint64_t[n];
    
    std::pair<double, double> elapsed = runSequential(n, aUnite, mUnite, mSameSet, resComponent, seed);
    double elapsedUnite = elapsed.first;
    double elapsedSameSet = elapsed.second;
    
    //for (uint64_t i = 0; i < n; i++) std::cout << resComponent[i];
    
    //    std::cout << "\n";
    
    std::cout<<"n="<<n<<" mUnite="<<mUnite<<" mSameSet="<<mSameSet<<std::endl;
    std::cout << "Time " << (elapsedUnite + elapsedSameSet) << "\n";
    std::cout << "TimeSpentOnUniteOperations " << elapsedUnite << "\n";
    std::cout << "TimeSpentOnSameSetSameSetOperations " << elapsedSameSet << "\n";

    GSTATS_PRINT;

    long long total_unite = GSTATS_GET_STAT_METRICS(op_unite, TOTAL)[0].sum;
    long long total_sameset = GSTATS_GET_STAT_METRICS(op_sameset, TOTAL)[0].sum;
    long long total_ops = total_unite + total_sameset;
    std::cout<<"unite throughput="<<(total_unite / elapsedUnite)<<std::endl;
    std::cout<<"sameset throughput="<<(total_sameset / elapsedSameSet)<<std::endl;
    std::cout<<"combined throughput="<<(total_ops / (elapsedSameSet + elapsedUnite))<<std::endl;

    std::cout<<std::endl;
    papi_print_counters(total_ops);
    
    delete [] aUnite;
    delete [] resComponent;
    papi_deinit_program();
    GSTATS_DESTROY;
    return 0;
}
