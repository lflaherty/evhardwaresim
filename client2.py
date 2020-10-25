#!/usr/bin/python3

from multiprocessing.connection import Client
import time

address = ('localhost', 6000)
conn = Client(address, authkey=b'secret password')

# conn.send('close')
# can also send arbitrary objects:
# conn.send(['a', 2.5, None, int, sum])
# conn.close()

while True:
    conn.send(('get', 'dt_now'))
    response = conn.recv() # Should be formatted as ('put', name, object)

    if response[0] == 'put':
        print('{}: {}'.format(response[1], response[2]))    

    time.sleep(1)