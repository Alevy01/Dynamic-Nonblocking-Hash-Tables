#include <cstdlib>
#include <iostream>
#include <signal.h>
#include <pthread.h>
#include <api/api.hpp>
#include <common/platform.hpp>
#include <common/locks.hpp>
#include "bmconfig.hpp"
#include "../include/api/api.hpp"
using namespace std;

const int THREAD_COUNT = 4;
const int NUM_TRANSACTIONS = 100000;

// shared variable that will be incremented by transactions
int x = 0;

Config::Config() :
    bmname(""),
    duration(1),
    execute(0),
    threads(THREAD_COUNT),
    nops_after_tx(0),
    elements(256),
    lookpct(34),
    inspct(66),
    sets(1),
    ops(1),
    time(0),
    running(true),
    txcount(0)
{
}

Config CFG TM_ALIGN(64);

void* run_thread(void* i) {
    // each thread must be initialized before running transactions
    TM_THREAD_INIT();

    for(int i=0; i<NUM_TRANSACTIONS; i++) {
        // mark the beginning of a transaction
        TM_BEGIN(atomic)
        {
            // add this memory location to the read set
            int z = TM_READ(x);
            // add this memory location to the write set
            TM_WRITE(x, z+1);
        }
        TM_END; // mark the end of the transaction
    }

    TM_THREAD_SHUTDOWN();
}

int main(int argc, char** argv) {
    TM_SYS_INIT();

    // original thread must be initalized also
    TM_THREAD_INIT();

    void* args[256];
    pthread_t tid[256];

    // set up configuration structs for the threads we'll create
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
    for (uint32_t i = 0; i < CFG.threads; i++)
        args[i] = (void*)i;

    // actually create the threads
    for (uint32_t j = 1; j < CFG.threads; j++)
        pthread_create(&tid[j], &attr, &run_thread, args[j]);

    // all of the other threads should be queued up, waiting to run the
    // benchmark, but they can't until this thread starts the benchmark
    // too...
    run_thread(args[0]);

    // everyone should be done.  Join all threads so we don't leave anything
    // hanging around
    for (uint32_t k = 1; k < CFG.threads; k++)
        pthread_join(tid[k], NULL);

    // And call sys shutdown stuff
    TM_SYS_SHUTDOWN();

    printf("x = %d\n", x); // x should equal (THREAD_COUNT * NUM_TRANSACTIONS)

    return 0;
}
