#!/usr/bin/python3

from threading import Thread
import logging
import datetime
import time
from DataStore import DataStore

from SimObjectExample1 import SimObjectExample1
from SimObjectExample2 import SimObjectExample2

# Setup logging
logging.basicConfig(level=logging.DEBUG,
                    format='[%(levelname)s] (%(threadName)-10s) %(message)s',
                    )


# Define sim objects here
sim_objects = [
    # (Name string, object)
    ('Example1', SimObjectExample1()),
    ('Example2', SimObjectExample2())
]

# Create Data Storage
data_store = DataStore()

# Create threads
threads = []
for obj in sim_objects:
    name = obj[0]
    sim_object = obj[1]
    sim_object.set_data_store(data_store)
    sim_object.init()
    threads.append(Thread(target=sim_object.run, name=name))

# Start threads and wait for finish
for t in threads:
    t.start()
for t in threads:
    t.join()