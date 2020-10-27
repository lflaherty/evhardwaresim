import logging
import datetime
import time

class SimObjectExample2:
    def __init__(self, data_store):
        self.data_store = data_store

    def run(self):
        while True:
            now_get = self.data_store.get('now')
            logging.info('now: {}'.format(now_get))    
            time.sleep(1)