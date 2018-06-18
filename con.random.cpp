#include <semaphore.h>
#include <time.h>
#include <map>
#include <queue>
#include <algorithm>
#include <ratio>
#include <chrono>
#include <errno.h>
#include <immintrin.h>
#include <iostream>
#include <string>
#include <sstream>
#include <iostream>
#include <random>
#include <stdio.h>
#include <thread>
#include <vector>
#include <ctime>
#include <atomic>
#include <fstream>
#include "node.h"
#include "operationscon.h"

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

//#include "allocation.h"

#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#endif

#define __HANDLE_STATS(handle_stat) \
    handle_stat(LONG_LONG, txn_commit, 1, { \
            stat_output_item(PRINT_RAW, SUM, TOTAL) \
    }) \
    handle_stat(LONG_LONG, txn_abort, 1, { \
            stat_output_item(PRINT_RAW, SUM, TOTAL) \
    }) \
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
Node * nodes;
//std::atomic<uint64_t> **p;
//volatile char padding1[192];
//uint64_t **id;
volatile char padding2[192];
uint64_t n;
volatile char padding3[192];
uint64_t maxCAS;
volatile char padding4[192];
int seed;
volatile char padding5[192];
std::atomic<bool> barrier;
volatile char padding6[192];
std::atomic<int> ready;
volatile char padding7[192];
std::atomic<bool> done;
volatile char padding8[192];


__thread int tid;

bool SameSet(uint64_t u, uint64_t v) {
    while (1) {
        u = Find(nodes, u, maxCAS);
        v = Find(nodes, v, maxCAS);
        if (u == v) return 1;
        uint64_t w = nodes[u].p.load(std::memory_order_relaxed);
        if (u == w) return 0;

    }
}

void Comp(uint64_t *res) //compute array of minimum index nodes in the components
{
    for (uint64_t i = 0; i < n; i++) res[i] = i;
    for (uint64_t i = 0; i < n; i++) {
        uint64_t j = i;
        while (nodes[j].p != j) j = nodes[j].p;
        if (res[j] > i) res[j] = i;
    }
    for (uint64_t i = 0; i < n; i++) {
        uint64_t j = i;
        while (nodes[j].p != j) j = nodes[j].p;
        res[i] = res[j];
    }
}

void Init(int seed) {
    //p = new std::atomic<uint64_t>[n];
    //id = new uint64_t[n];

    nodes = new Node[n];
//    p = static_cast<std::atomic<uint64_t>**> (memalloc::CallocAligned(n, sizeof
//                (std::atomic<uint64_t>*), 64));
//    id = static_cast<uint64_t**> (memalloc::CallocAligned(n, sizeof (uint64_t*), 64));

     for (uint64_t i = 0; i < n; i++)
     {
         nodes[i].p = i;
//         p[i] = static_cast<std::atomic<uint64_t>*>(memalloc::get<std::atomic<uint64_t>>(memalloc::kCachePrefetch * 4));
//         id[i] = static_cast<uint64_t*>(memalloc::get<uint64_t>(memalloc::kCachePrefetch * 4));
//        p[i]->store(i, std::memory_order_relaxed);
//        *id[i] = 0;
    }
    std::mt19937 rng;
    rng.seed(seed);
    //create a random permutation
    for (uint64_t i = 0;  i < n; i++) nodes[i].id = i;
    for (uint64_t i = 0; i < n; i++)
    {
        std::uniform_int_distribution<uint64_t> rnd_st(i, n - 1);
        uint64_t j = rnd_st(rng);
        uint64_t temp = nodes[i].id;
        nodes[i].id = nodes[j].id;
        nodes[j].id = temp;
    }
}

bool Unite(uint64_t u, uint64_t v) {
    int cntfail = 0;
    while (1) {
        u = Find(nodes, u, maxCAS);
        v = Find(nodes, v, maxCAS);
        if (u == v) return 1;

        if (nodes[u].id < nodes[v].id) {
                if (nodes[u].p.compare_exchange_strong(u, v))
                {
                    GSTATS_ADD(tid, txn_commit, 1);
                    return 0;
                }
                else
                {
                    GSTATS_ADD(tid, txn_abort, 1);
                }
            
        } else {
            
                if (nodes[v].p.compare_exchange_strong(v, u))
                {
                    GSTATS_ADD(tid, txn_commit, 1);
                    return 0;
                }
                else
                {
                    GSTATS_ADD(tid, txn_abort, 1);
                }
        }
    }
}

double
timediff_in_s(const struct timespec &start,
            const struct timespec &end) {
    struct timespec tmp;
    if (end.tv_nsec < start.tv_nsec) {
        tmp.tv_sec = end.tv_sec - start.tv_sec - 1;
        tmp.tv_nsec = 1000000000 + end.tv_nsec - start.tv_nsec;
    } else {
        tmp.tv_sec = end.tv_sec - start.tv_sec;
        tmp.tv_nsec = end.tv_nsec - start.tv_nsec;
    }

    return tmp.tv_sec + (double) tmp.tv_nsec / 1000000000.0;
}

void get_monotonic_time(struct timespec *curtime) {
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

void runThreadsS(const int __tid, uint64_t len, uint64_t n, int seed) //run SaneSet commands
{
    tid = __tid;
    papi_create_eventset(tid);
    std::mt19937 rng;
    rng.seed(seed);

    std::uniform_int_distribution<uint64_t> rnd_st(0, n - 1);

    ready.fetch_add(1);
    while (!barrier.load()) {
        // wait for main thread
    }
    papi_start_counters(tid);

    for (uint64_t i = 0; i < len; i++) {
        uint64_t u, v;
        u = rnd_st(rng);
        v = rnd_st(rng);
        SameSet(u, v);
        GSTATS_ADD(tid, op_sameset, 1);
        if (done.load(std::memory_order_relaxed)) break;
    }
    done.store(1);
    ready.fetch_add(-1);
    while (ready.load() > 0) {
        // wait until all threads finish
    }
    papi_stop_counters(tid);
}

void runThreadsU(const int __tid, op::Operation * a, uint64_t l, uint64_t r) //run Unite commands
{
    tid = __tid;
    papi_create_eventset(tid);
    ready.fetch_add(1);
    while (!barrier.load()) {
        // wait for main thread
    }
    papi_start_counters(tid);
    
    for (uint64_t i = l; i <= r; i++) {
        Unite(a[i].u, a[i].v);
        GSTATS_ADD(tid, op_unite, 1);
        if (done.load(std::memory_order_relaxed)) break;
    }
    done.store(1);
    ready.fetch_add(-1);
    while (ready.load() > 0) {
        // wait until all threads finish
    }
    papi_stop_counters(tid);
}

double mainThreads(op::Operation * a, uint64_t n, uint64_t m, int nthreads, int seed) {
    done.store(0);
    ready.store(0);
    barrier.store(0);

    std::vector<std::thread> threads(MAX_TID_POW2);

    uint64_t len = m / nthreads + 1;

    if (m % nthreads == 0) len--;

    uint64_t l = 0, r = len - 1;


    for (int i = 0; i < nthreads; i++) {
        if (r >= m) r = m - 1;
        if (a != NULL) threads[i] = std::thread(runThreadsU, i, a, l, r); //run Unites
        else threads[i] = std::thread(runThreadsS, i, r - l + 1, n, seed); //run SameSets
        l = r + 1;
        r = r + len;
        seed++;
    }

    /* Begin benchmark. */

    struct timespec start, end;
    //clock_gettime(CLOCK_MONOTONIC, &start);
    __sync_synchronize();
    while (ready.load() < nthreads) {
        // wait until threads have performed their initialization
        timespec tsExpected;
        tsExpected.tv_sec = 0;
        tsExpected.tv_nsec = 10 /* millis */ * ((__syscall_slong_t) 1000000);
        nanosleep(&tsExpected, NULL);
    }
    __sync_synchronize();
    // start the run
    get_monotonic_time(&start);
    barrier.store(1);
    __sync_synchronize();

    while (!done.load()) {
        // wait for the first thread to finish and set done
        timespec tsExpected;
        tsExpected.tv_sec = 0;
        tsExpected.tv_nsec = 10 /* millis */ * ((__syscall_slong_t) 1000000);
        nanosleep(&tsExpected, NULL);
    }

    __sync_synchronize();
    //clock_gettime(CLOCK_MONOTONIC, &end);
    get_monotonic_time(&end);
    __sync_synchronize();
    
    for (int i = 0; i < nthreads; i++) {       
        threads[i].join();
    }

    /* End benchmark */
    return timediff_in_s(start, end);
}

int main() {

    uint64_t mSameSet, mUnite, mExtraUnite, nExtraUnite;
    int nthreads;
    std::string inputstr;
    
    inputstr="roadNet-CA.txt;";
    std::cin >> mSameSet >> nthreads >> seed;

    // setup per-thread statistics
    GSTATS_CREATE_ALL;
    GSTATS_CLEAR_ALL;

    papi_init_program(MAX_TID_POW2);
                
    op::Operation * o = op::getOperations(inputstr, n, mUnite);

    Init(seed);

    double elapsedUnite = mainThreads(o, n, mUnite, nthreads, seed);

//    uint64_t **resComponent = static_cast<uint64_t**> (memalloc::CallocAligned(n, sizeof (uint64_t*), memalloc::kCachePrefetch * 4));
//
//    for (uint64_t i = 0; i < n; i++) resComponent[i] = static_cast<uint64_t*> (memalloc::get<uint64_t>(memalloc::kCachePrefetch * 4));
    uint64_t * resComponent = new uint64_t[n];
    Comp(resComponent);

    double elapsedSameSet = mainThreads(NULL, n, mSameSet, nthreads, seed);

    //for (uint64_t i = 0; i < n; i++) std::cout << *resComponent[i];

    //std::cout << "\n";
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
    
//    for (uint64_t i = 0; i < n; i++) {
//        free(resComponent[i]);
////        free(p[i]);
////        free(id[i]);
//    }
//    for (uint64_t i = 0; i < mUnite; i++) free(o[i]);

//    free(resComponent);
    delete[] resComponent;
    delete[] nodes;
    delete[] o;
//    free(p);
//    free(id);
//    free(o);

    papi_deinit_program();
    GSTATS_DESTROY;
    return 0;
}
