import logging
import datetime
import time

from SimObject import SimObject

class SimObjectExample1(SimObject):
    def __init__(self):
        SimObject.__init__(self, 0)
        self.dt_count = None

    def init(self):
        self.data_store.set('counter', 0)
        self.data_store.set('now', datetime.datetime.now())

    def step(self, dt):
        now = datetime.datetime.now()
        self.data_store.set('now', now)

        counter = self.data_store.get('counter')
        counter += 1
        self.data_store.set('counter', counter)