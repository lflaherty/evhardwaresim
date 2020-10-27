#!/usr/bin/python3

from threading import Thread
import logging
import datetime
import time
from DataStore import DataStore

# Setup logging
logging.basicConfig(level=logging.DEBUG,
                    format='[%(levelname)s] (%(threadName)-10s) %(message)s',
                    )

#################################################
prev_now = None
counter = 0
def client1(data_store):
    while True:
        now = datetime.datetime.now()
        data_store.set('now', now)

        global counter
        global prev_now
        if prev_now is None:
            prev_now = now

        dt = now - prev_now
        counter += 1
        if dt.seconds >= 1:
            logging.info('Updated {} times'.format(counter))
            counter = 0
            prev_now = now

#################################################
def client2(data_store):
    while True:
        now_get = data_store.get('now')
        logging.info('now: {}'.format(now_get))    
        time.sleep(1)

# Create Data Storage
dataStore = DataStore()

# Create threads
threads = []
threads.append(Thread(target=client1, name='Client1', args=(dataStore,)))
threads.append(Thread(target=client2, name='Client2', args=(dataStore,)))

# Start threads and wait for finish
for t in threads:
    t.start()
for t in threads:
    t.join()