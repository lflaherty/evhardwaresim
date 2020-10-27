import logging
import time
import datetime

from abc import ABC, abstractmethod

class SimObject(ABC):
    def __init__(self, dt):
        self.dt = dt
        self.data_store = None
        self.prev_now = None
    
    def set_data_store(self, data_store):
        self.data_store = data_store
    
    def run(self):
        if self.data_store is None:
            logging.critical('data_store object not set!')
            return
        
        while True:
            # Calculate delta time
            if self.prev_now is None:
                self.prev_now = datetime.datetime.now()
            dt = datetime.datetime.now() - self.prev_now

            self.step(dt)
            self.prev_now = datetime.datetime.now()
            if self.dt > 0:
                time.sleep(self.dt)
    
    """
    Run once at the beginning, after data_store is set
    """
    @abstractmethod
    def init(self):
        pass

    """
    Just need to override this in the children
    dt  Measured delta time
    """
    @abstractmethod
    def step(self, dt):
        pass