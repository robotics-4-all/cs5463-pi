import datetime
import os
import json
import requests as req
import time
import math
import pytz
from datetime import timezone
import sys
import socket
import re


UNIX_SOCKET_FILE = '/tmp/cs5463.sock'

CENOTE_ENDPOINT = "https://cenote.sidero.services/api/projects/pid7870cd27f3c/events/integration?writeKey=908bfd44-e91e-4b0f-9c40-972801126e09"
headers = {'content-type': 'application/json'}

# Currently timeout does not work as expected - python requests issue?
CENOTE_CONNECTION_TIMEOUT = 2
CENOTE_READ_TIMEOUT = 2


def construct_batch_payload(batch):
    payload = {}
    payload["payload"] = batch
    return payload


def calculate_reactive_power(p,angle):
    q = p*math.tan(angle*math.pi/180)
    return q


def run_forever(installationid):
    try:
        os.unlink(UNIX_SOCKET_FILE)
    except OSError:
        if os.path.exists(UNIX_SOCKET_FILE):
            raise
    sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
    # Bind socket to file descriptor
    sock.bind(UNIX_SOCKET_FILE)
    sock.listen(1)
    print("[*] - Waiting for connection...")
    connection, client_address = sock.accept()
    print("[*] - Client connected: {}".format(client_address))

    # cache
    cenote_data = []

    # Receive the data in small chunks and retransmit it
    while True:
        data = connection.recv(4096)
        if not data:
            break
        try:
            data = data.decode('utf8')
            data = re.sub('\s+', '', data)
            data = data[:-1]
            data = data.replace('}\x00{', '},{')
            data = '[{}]'.format(data)
            data = json.loads(data)
        except Exception as e:
            print('[*] - Error while parsing data from socket!')
            continue
        # print(cenote_data)

        if isinstance(data, dict):
            data = [data]
        cenote_data = cenote_data + data
        for i in range(len(cenote_data)):
            # CAN GET FROM THE CS5463 DRIVER!!!
            reactive = calculate_reactive_power(
                cenote_data[i]["active"],
                cenote_data[i]["phaseanglecurrentvoltagel1"]
            )
            cenote_data[i]["reactive"] = reactive
            timestamp = datetime.datetime.now(pytz.utc)
            temp_obj = {}
            temp_obj["data"] = cenote_data[i]
            temp_obj["data"]["installationid"] = installationid
            temp_obj["timestamp"] = timestamp.replace(
                tzinfo=timezone.utc).timestamp()*1000
        data = construct_batch_payload(cenote_data)
        json_data = json.dumps(data)
        print("[*] - Sending Batch of {}:".format(len(cenote_data)))
        print(data)
        try:
            response = req.post(
                CENOTE_ENDPOINT,
                data=json_data,
                headers=headers,
                timeout=(CENOTE_CONNECTION_TIMEOUT, CENOTE_READ_TIMEOUT)
            )
            print(response.content)
            print(timestamp, "  ", timestamp.replace(
                tzinfo=timezone.utc).timestamp()*1000)
            # In case of succesfull push of data, clear the buffer so
            # the batch won't be send again
            cenote_data = []
        except req.exceptions.ConnectionError as e:
            print("[*] - HTTP Connection Error. Could not connect to the platform")


def main(id):
    run_forever(id)


if __name__ == "__main__":
    main(sys.argv[1])

