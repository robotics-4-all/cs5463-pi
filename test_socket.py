#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import socket
import os
# from time import sleep

UNIX_SOCKET_FILE = '/tmp/cs5463.sock'

if __name__ == "__main__":
    try:
        os.unlink(UNIX_SOCKET_FILE)
    except OSError:
        if os.path.exists(UNIX_SOCKET_FILE):
            raise
    sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
    # Bind socket to file descriptor
    sock.bind(UNIX_SOCKET_FILE)
    sock.listen(1)
    connection, client_address = sock.accept()
    print("Client connected: {}".format(client_address))

    # Receive the data in small chunks and retransmit it
    while True:
        data = connection.recv(1024)
        if not data:
            break
        print('Received data: {}'.format(data))
