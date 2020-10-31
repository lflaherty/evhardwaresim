#include <iostream>

#include <vector>
#include <memory>
#include <pthread.h>
#include "DataStore.h"
#include <sys/mman.h>    // for mlockall

#include "SimObjectExample1.h"
#include "SimObjectExample2.h"

void declareSimObjects(
        DataStore& dataStore, 
        std::vector<std::shared_ptr<SimObject>>& simObjects)
{
    using namespace std;

    // Create all of the sim objects here and append to the simObjects vector
    simObjects.push_back(make_shared<SimObjectExample1>(dataStore));
    simObjects.push_back(make_shared<SimObjectExample2>(dataStore));
}

/**
 * Setup for Real-Time operation.
 * Locks memory and sets RR scheduling
 */
void setupRT()
{
	// Lock memory to ensure no swapping is done.
	if(mlockall(MCL_FUTURE|MCL_CURRENT)) {
		fprintf(stderr,"WARNING: Failed to lock memory (try running as superuser?)\n");
	} else {
		printf("Successfully locked memory\n");
	}

	// Set our thread to real time priority
	struct sched_param sp;
	sp.sched_priority = 30;
	if(pthread_setschedparam(pthread_self(), SCHED_RR, &sp)) {
		fprintf(stderr,"WARNING: Failed to thread to real-time priority (try running as superuser?)\n");
	} else {
		printf("Successfully set thread to real-time priority\n");
	}
}

int main()
{
    using namespace std;

    // Delcare data store
    DataStore dataStore;

    // Delcare sim objects
    vector<shared_ptr<SimObject>> objects;
    declareSimObjects(dataStore, objects);

    // Start
    setupRT();
        
    // setup params
    struct sched_param param;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setschedpolicy(&attr, SCHED_RR);
    param.sched_priority = 0;
    pthread_attr_setschedparam(&attr, &param);

    vector<shared_ptr<pthread_t>> threads;
    for (shared_ptr<SimObject> obj : objects)
    {
        // initialize
        obj->init();

        // create
        shared_ptr<pthread_t> thd = make_shared<pthread_t>();
        pthread_create(thd.get(), &attr, obj->run, obj.get());
        threads.push_back(thd);
    }

    // Wait for threads to finish
    for (shared_ptr<pthread_t> thd : threads)
    {
        pthread_join(*thd, NULL);
    }

    return 0;
}