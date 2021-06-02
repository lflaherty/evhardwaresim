#include <iostream>

#include <vector>
#include <memory>
#include <pthread.h>
#include "DataStore.h"
#include <sys/mman.h>    // for mlockall
#include <unistd.h> 

#include "CANInterface.h"
#include "GPIOInterface.h"
#include "SPIInterface.h"
#include "UARTInterface.h"

#include "MCP4912.h"

#include "SimObjectExample1.h"
#include "PrintTask.h"

// Startup options
static bool g_clearOnPeriodicPrint = false;

void declareSimObjects(
        DataStore& dataStore, 
        std::vector<std::shared_ptr<SimObject>>& simObjects)
{
    using namespace std;

    // Create all of the sim objects here and append to the simObjects vector
    // Hardware Interfaces
    shared_ptr<CANInterface> canInterface(new CANInterface(dataStore));
    shared_ptr<UARTInterface> uartInterface(new UARTInterface(dataStore));
    shared_ptr<GPIOInterface> gpioInterface(new GPIOInterface(dataStore));

    SPIInterface::SPIConfig spiConfig;
    spiConfig.device = "/dev/spidev1.0";
    spiConfig.mode = 0;
    spiConfig.bits = 8;
    spiConfig.firstBit = 0; // MSB first
    spiConfig.speed = 1600000;
    shared_ptr<SPIInterface> spiInterface(new SPIInterface(dataStore, gpioInterface, spiConfig));

    shared_ptr<MCP4912> mcp4912(new MCP4912(dataStore, gpioInterface, spiInterface));

    // Sim Objects
    shared_ptr<SimObjectExample1> example(new SimObjectExample1(dataStore, mcp4912, canInterface));
    shared_ptr<PrintTask> printTask(new PrintTask(dataStore, canInterface, uartInterface));
    printTask->setClearOnPeriodicPrint(g_clearOnPeriodicPrint);

    simObjects.push_back(example);
    simObjects.push_back(canInterface);
    simObjects.push_back(uartInterface);
    simObjects.push_back(gpioInterface);
    simObjects.push_back(spiInterface);
    simObjects.push_back(mcp4912);
    simObjects.push_back(printTask);
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
	sp.sched_priority = 1;
	if(pthread_setschedparam(pthread_self(), SCHED_RR, &sp)) {
		fprintf(stderr,"WARNING: Failed to thread to real-time priority (try running as superuser?)\n");
	} else {
		printf("Successfully set thread to real-time priority\n");
	}
}

int main(int argc, char* argv[])
{
    using namespace std;

    // Get args
    int opt;
    while ((opt = getopt(argc, argv, "c")) != -1) {
        switch (opt)
        {
        case 'c':
            g_clearOnPeriodicPrint = true;
            break;
        case 'h':
        default:
            cerr << "Usage: " << argv[0] << " [-c]" << endl;
            cerr << "Parameters:" << endl;
            cerr << "\t-c\tClear screen on each refresh" << endl;
            return 1;
        }
    }

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