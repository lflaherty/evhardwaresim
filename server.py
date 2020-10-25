#!/usr/bin/python3

from multiprocessing.connection import Listener
from threading import Thread, Lock

data_store = {}
data_store_lock = Lock()

"""
Wait for data on a connection
"""
def connection(conn):
    while True:
        msg = conn.recv()
        if len(msg) == 0:
            continue

        instruction = msg[0]

        if instruction == 'close':
            conn.close()
            break
        elif instruction == 'get':
            # msg should be ('get', name)

            # Get data
            name = msg[1]
            data_store_lock.acquire()
            try:
                if name in data_store:
                    data = data_store[name]
                else:
                    data = None
            finally:
                data_store_lock.release()

            # Send data back
            send_msg = ('put', name, data)
            conn.send(send_msg)
        elif instruction == 'put':
            # msg should be ('put', name, object)

            # Get data
            name = msg[1]
            data = msg[2]

            # Store data
            data_store_lock.acquire()
            try:
                data_store[name] = data
            finally:
                data_store_lock.release()

"""
Listen for connections
"""
def listener():
    address = ('localhost', 6000)
    listener = Listener(address, authkey=b'secret password')

    while True:
        conn = listener.accept()
        print('connection accepted from', listener.last_accepted)

        t = Thread(target=connection, args=(conn,))
        t.start()

    listener.close()


if __name__ == '__main__':
    listener()