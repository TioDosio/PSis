import zmq
import sys
from messages_pb2 import ClientMessage, ResponseMessage

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

class Entity:
    def __init__(self):
        self.entity_type = None  # type of entity
        self.ch = None  # ch of entity
        self.points = None  # amount of points
        self.pos_x = None  # position of entity
        self.pos_y = None  # position of entity
        self.direction = None  # direction of last movement
        self.secret_code = None  # secret code to send

class ClientMessage:
    def __init__(self):
        self.entity_type = None  # type of entity
        self.msg_type = None  # type of message
        self.content = None  # value to send (points, char, movement direction, etc)
        self.secret_code = None  # secret id for entity

class ResponseMessage:
    def __init__(self):
        self.success = None  # 0 - fail, 1 - success, 2 - success in disconnection
        self.score = None  # score of the player
        self.secret_code = None  # secret id of entity

def send_and_receive_message(entity_type, msg_type, content, secret_code):
    # Create a ClientMessage
    client_msg = ClientMessage()
    client_msg.entity_type = entity_type
    client_msg.msg_type = msg_type
    client_msg.content = content
    client_msg.secret_code = secret_code

    # Serialize the message
    msg_bytes = client_msg.SerializeToString()

    # Send the message
    socket.send(msg_bytes)

    print(f"Sent {msg_type} message to the server")

    # Receive the response
    resp_bytes = socket.recv()

    # Parse the response
    response_msg = ResponseMessage()
    response_msg.ParseFromString(resp_bytes)

    print("Received response from the server:")
    print(f"Success: {response_msg.success}")
    print(f"Score: {response_msg.score}")
    print(f"Secret code: {response_msg.secret_code}")
    

def main(server_ip, server_port):
    # Connect to the server
    context = zmq.Context()
    socket = context.socket(zmq.REQ)
    socket.connect(f"tcp://{server_ip}:{server_port}")









    entity_type = 1  # Update with the desired entity type
    msg_type = 0  # Update with the desired message type
    content = "Some content"
    secret_code = 123
    send_and_receive_message(entity_type, msg_type, content, secret_code)

    socket.close()



if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python client.py <server_ip> <server_port>")
        sys.exit(1)

    server_ip = sys.argv[1]
    server_port = sys.argv[2]
    main(server_ip, server_port)
    
