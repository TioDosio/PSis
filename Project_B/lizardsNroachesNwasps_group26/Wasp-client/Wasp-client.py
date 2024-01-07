import zmq
import sys
import argparse
import random
import time
import messages_pb2

class EntityType:
    LIZARD = 0
    ROACH = 1
    WASP = 2

class Direction:
    UP = 0
    DOWN = 1
    LEFT = 2
    RIGHT = 3

class MessageType:
    CONNECT = 0
    MOVE = 1
    DISCONNECT = 2

class ClientMessage:
    def __init__(self):
        self.message = messages_pb2.ClientMessage()

class ResponseMessage:
    def __init__(self):
        self.success = None
        self.score = None
        self.secret_code = None

def main(server_ip, server_port, n_wasps=None):
    if n_wasps < 1 or n_wasps > 10:
        print("Invalid number of Wasps (1-10).")
        exit(0)

    # Connect to the server
    context = zmq.Context()
    socket = context.socket(zmq.REQ)
    socket.connect(f"tcp://{server_ip}:{server_port}")
    client_id = -1
    wasps_codes = [0] * n_wasps
    for i in range(n_wasps):
        client_message = ClientMessage()
        client_message.message.entity_type = EntityType.WASP
        client_message.message.msg_type = MessageType.CONNECT
        client_message.message.secret_code = 0
        client_message.message.content = 0
        client_message.message.client_code = client_id

        msg = client_message.message.SerializeToString()
        socket.send(msg)
        serialized_message = socket.recv()

        response_message = messages_pb2.ResponseMessage()
        response_message.ParseFromString(serialized_message)
        if (client_id == -1):
            client_id = response_message.secret_code
        if response_message.success == 0:
            exit(0)
        wasps_codes[i] = response_message.secret_code

    while True:
        for i in range(n_wasps):
            time.sleep(random.uniform(0, 0.6))
            client_message = ClientMessage()
            client_message.message.entity_type = EntityType.WASP
            client_message.message.msg_type = MessageType.MOVE  # Assuming you want to send a MOVE message
            client_message.message.content = random.choice([Direction.UP, Direction.DOWN, Direction.LEFT, Direction.RIGHT])
            client_message.message.secret_code = wasps_codes[i]
            client_message.message.client_code = client_id
            
            if client_message.message.content == Direction.LEFT:
                print(" Going Left")
            elif client_message.message.content == Direction.RIGHT:
                print(" Going Right")
            elif client_message.message.content == Direction.DOWN:
                print(" Going Down")
            elif client_message.message.content == Direction.UP:
                print(" Going Up")

            msg = client_message.message.SerializeToString()
            socket.send(msg)
            serialized_message = socket.recv()

            response_message = messages_pb2.ResponseMessage()
            response_message.ParseFromString(serialized_message)

            if (response_message.success == 0):
                    print("Timeout")
                    exit(0)

    socket.close()

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Your script description here")
    parser.add_argument("server_ip", type=str, help="Server IP")
    parser.add_argument("server_port", type=str, help="Server Port")
    parser.add_argument(
        "n_wasps",
        nargs="?",
        type=int,
        help="Number of Wasps (1-10) default=random(1,10) ",
        default=random.randint(1, 10),
    )
    args = parser.parse_args()
    main(args.server_ip, args.server_port, args.n_wasps)
