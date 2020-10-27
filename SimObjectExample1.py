import logging
import datetime
import time

class SimObjectExample1:
    def __init__(self, data_store):
        self.data_store = data_store
        self.prev_now = None
        self.counter = 0

    def run(self):
        while True:
            now = datetime.datetime.now()
            self.data_store.set('now', now)

            if self.prev_now is None:
                self.prev_now = now

            dt = now - self.prev_now
            self.counter += 1
            if dt.seconds >= 1:
                logging.info('Updated {} times'.format(self.counter))
                self.counter = 0
                self.prev_now = now