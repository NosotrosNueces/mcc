#!/usr/bin/python3
"""Host a server that sends data specified by some JSON file.
"""
import sys
import json
import time
import socket
import base64

HOST = '' # symbolic name for all available interfaces
PORT = 25565 # default minecraft port


def host_debug_server(host, port, data):
    """Send a non-zero number of packets to the connecting client.
    """
    data_offset = data[0]['timestamp']
    server_offset = time.time() * 10**9

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.bind((host, port))
        s.listen(1)
        conn, addr = s.accept()

        with conn:
            print('Connected to', addr)
            for packet in data:
                ts = packet['timestamp']
                direction = packet['bountTo']
                payload = packet['payload']

                if direction != "clientbound":
                    continue

                # busy wait is the only accurate wait
                while time.time() * 10**9 - server_offset < ts - data_offset:
                    pass

                conn.sendall(base64.b64decode(payload))
                print("sent: ", payload)


if __name__ == "__main__":
    # parse cmd args
    if len(sys.argv) < 2:
        print("Usage: debug_server.py <json data> [port]")
        exit(1)
    JSON_PATH = sys.argv[1]

    if len(sys.argv) >= 3:
        PORT = int(sys.argv[2])


    # load JSON data
    with open(JSON_PATH) as json_data:
        DATA = json.load(json_data)['packets']


    # run mock server
    host_debug_server(HOST, PORT, DATA)
