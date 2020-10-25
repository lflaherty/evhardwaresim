#!/usr/bin/python3

from multiprocessing.connection import Client
import datetime

address = ('localhost', 6000)
conn = Client(address, authkey=b'secret password')
# conn.send('close')
# can also send arbitrary objects:
# conn.send(['a', 2.5, None, int, sum])

prev_now = None
counter = 0
while True:
    now = datetime.datetime.now()

    conn.send(('put', 'dt_now', now))

    if prev_now is None:
        prev_now = now

    dt = now - prev_now
    counter += 1
    if dt.seconds >= 1:
        print('Updated {} times'.format(counter))
        counter = 0
        prev_now = now

conn.close()