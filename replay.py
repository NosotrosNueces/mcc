#!/usr/bin/python3
"""Host a server that sends data specified by some JSON file.
"""
import json
import time
import socket
import base64

DEFAULT_PORT = 25565 # default minecraft port
DEFAULT_SPACING = 100000 # default ns interval for compressed replay (.1ms)


def ns_time():
    return time.time() * 10**9


def host_debug_server(host, port, data):
    """Send a non-zero number of packets to the connecting client.
    """
    if type(data) == list:
        data_offset = data[0]['timestamp']
    else:
        data_offset = 0 # because generator
    server_offset = ns_time()

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
                while ns_time() - server_offset < ts - data_offset:
                    pass

                conn.sendall(base64.b64decode(payload))
                print("sent: ", payload)


def _compress(index, data, spacing=DEFAULT_SPACING):
    data['timestamp'] = index * spacing
    return data


if __name__ == "__main__":
    import argparse

    parser = argparse.ArgumentParser()
    parser.add_argument("data", help="path to json data file")
    parser.add_argument("-p", "--port",
                        help="port to listen from (default 25565)",
                        type=int)
    parser.add_argument("--fast",
                        help="use fast replay interval (1packet/500ns)",
                        action='store_true')
    args = parser.parse_args()

    _json_path = args.data
    _port = args.port if args.port else DEFAULT_PORT


    # load data and compress if needed
    with open(_json_path) as json_data:
        _data = json.load(json_data)['packets']
    if args.fast:
        _data = (_compress(i, pkt, args.fast) for i, pkt in enumerate(_data))


    # listen on all available interfaces
    host_debug_server('', _port, _data)
