#include <iostream>

#include <vector>
#include <memory>
#include <pthread.h>
#include "DataStore.h"

#include "SimObjectExample1.h"
#include "SimObjectExample2.h"

int main()
{
    using namespace std;

    // Delcare data store
    DataStore dataStore;

    // Delcare sim objects
    vector<shared_ptr<SimObject>> objects;

    objects.push_back(make_shared<SimObjectExample1>(dataStore));
    objects.push_back(make_shared<SimObjectExample2>(dataStore));


    // Start
    vector<shared_ptr<pthread_t>> threads;
    for (shared_ptr<SimObject> obj : objects)
    {
        // initialize
        obj->init();

        // create
        shared_ptr<pthread_t> thd = make_shared<pthread_t>();
        pthread_create(thd.get(), NULL, obj->run, obj.get());
        threads.push_back(thd);
    }

    // Wait for threads to finish
    for (shared_ptr<pthread_t> thd : threads)
    {
        pthread_join(*thd, NULL);
    }

    return 0;
}