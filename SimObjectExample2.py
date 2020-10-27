import logging
import datetime
import time

from SimObject import SimObject

class SimObjectExample2(SimObject):
    def __init__(self):
        SimObject.__init__(self, 1)

    def init(self):
        self.data_store.set('counter', 0)
        self.data_store.set('now', datetime.datetime.now())

    def step(self, dt):
        now = self.data_store.get('now')
        counter = self.data_store.get('counter')
        logging.info('now: {}\tcounter: {}'.format(now, counter))

        self.data_store.set('counter', 0) # Reset