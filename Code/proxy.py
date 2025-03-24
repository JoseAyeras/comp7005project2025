import argparse
import sys

# attempt to comply with proxy design state machine
# handle arguments state
#def handle_args_state():
    # what do I do here?
    
    
parser = argparse.ArgumentParser()
server_ip = "192.168.0.1"
port = "7005"
drop_chance = 0.0
delay_chance = 0.0
delay_min = 0.0
delay_max = 0.0

parser.add_argument("-i", dest=server_ip, required=True)
parser.add_argument("-p", dest=port, required=True)
parser.add_argument("--drop-chance", dest=drop_chance, required=True)
parser.add_argument("--delay-chance", dest=delay_chance, required=True)
parser.add_argument("--delay-min", dest=delay_min, required=True)
parser.add_argument("--delay-max", dest=delay_max, required=True)
parser.parse_args(sys.argv)