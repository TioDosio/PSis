import zmq
import struct
import sys

class EntityType:
    LIZARD = 0
    ROACH = 1
    DISPLAY = 2

class Direction:
    UP = 0
    DOWN = 1
    LEFT = 2
    RIGHT = 3

class Entity:
    def __init__(self, entity_type, ch, points, pos_x, pos_y, direction, secret_code):
        self.entity_type = entity_type
        self.ch = ch
        self.points = points
        self.pos_x = pos_x
        self.pos_y = pos_y
        self.direction = direction
        self.secret_code = secret_code

class GenericMsg:
    def __init__(self, entity_type, msg_type, ch, direction, secret_code):
        self.entity_type = entity_type
        self.msg_type = msg_type
        self.ch = ch
        self.direction = direction
        self.secret_code = secret_code

def send_generic_msg(socket, generic_msg):
    packed_data = struct.pack('I B c B H H B h',
        generic_msg.entity_type,
        generic_msg.msg_type,
        bytes([ord(generic_msg.ch)]),
        generic_msg.direction,
        0,  # Padding byte for alignment
        0,  # Padding short int for alignment
        0,  # Padding byte for alignment
        generic_msg.secret_code
    )
    socket.send(packed_data)

def main(server_ip, server_port):
    try:
        # Convert server_port to an integer
        server_port = int(server_port)
    except ValueError:
        print("Error: Invalid server_port. Please provide a valid integer.")
        sys.exit(1)

    # Define the server address
    server_address = f"tcp://{server_ip}:{server_port}"

    # Create a ZeroMQ context and socket
    context = zmq.Context()
    socket = context.socket(zmq.PUSH)

    # Connect to the server
    socket.connect(server_address)

    # Create a generic_msg instance and send it
    generic_message = GenericMsg(
        entity_type=EntityType.ROACH,
        msg_type=1,  # 0 - connect, 1 - move, 2 - disconnect
        ch='A',      # Your value to send (points of roaches or char of lizard)
        direction=Direction.RIGHT,
        secret_code=123
    )

    # Send the generic message
    send_generic_msg(socket, generic_message)

    # Close the socket and context
    socket.close()
    context.term()

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python client.py <server_ip> <server_port>")
        sys.exit(1)

    server_ip = sys.argv[1]
    server_port = sys.argv[2]

    main(server_ip, server_port)
