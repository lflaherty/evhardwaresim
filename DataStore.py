
from threading import Lock

class DataStore:
    def __init__(self):
        self.data_store = {}
        self.lock = Lock()
    
    def get(self, name):
        self.lock.acquire()
        data = None
        try:
            if name in self.data_store:
                data = self.data_store[name]
        finally:
            self.lock.release()
        
        return data
    
    def set(self, name, data):
        self.lock.acquire()
        try:
            self.data_store[name] = data
        finally:
            self.lock.release()