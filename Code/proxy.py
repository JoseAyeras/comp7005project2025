import argparse
import sys
import random
from enum import Enum

class ExecState(Enum):
    handle_args = 0
    idle        = 1
    recv_packet = 2
    cleanup     = 3
    #states not shown from design diagram: delay, transmit
    #   moving from delay to transmit state is too simple
    #   to be worth turning into separate blocks,
    #   and those states are in their own threads for each packet

#global variables
    #server ip address, must be stored as a string due to python's udp functions I guess
server_ip = "::1"
    #listening port, must replicate the same listening port as the server to be effective
port = 7005
    #drop chance
drop_chance = 0.0
    #delay chance
delay_chance = 0.0
    #minimum delay time (delay will be a random number from min to max)
min_delay = 0.0
    #maximum delay time
max_delay = 0.0
    #port to client ip map (whenever server gives packet to the port, relay to the client, and vice versa)
    # Python3 code to demonstrate use of
    # {} symbol to initialize dictionary
    # Credit: geeks for geeks
port_map = {}
    #good thing I caught that these global vars were indented wrong before I executed

def packet_thread_exec():
    #drop check: if roll equal or below drop chance (which is a float from 0 to 1) then return (and therefore drop the packet)
    
    print("executing packet_thread_exec")

# attempt to comply with proxy design state machine
# handle arguments state
def handle_args_state():
    # what do I do here?
    
    
    parser = argparse.ArgumentParser()
    #server_ip   # = "192.168.0.1"
    #port        # = 7005
    #drop_chance # = 0.0
    #delay_chance# = 0.0
    #delay_min   # = 0.0
    #delay_max   # = 0.0
    
    parser.add_argument("--ip", required=True)                            #server ip
    parser.add_argument("--port", required=False, type=int)                 #port (maybe we won't use this)
    parser.add_argument("--drop-chance", required=True, type=float)     #drop chance
    parser.add_argument("--delay-chance", required=True, type=float)    #delay chance
    parser.add_argument("--delay-min", required=True, type=float)       #minimum delay time (delay will be a random number from min to max)
    parser.add_argument("--delay-max", required=False, type=float)      #maximum delay time, default is equal to min delay time, needs comparison check
    #args = sys.argv
    #for arg in args:
    #    parser.parse_args(arg)
    #ArgumentParser.parse_args(args=None, namespace=None)
    #args - List of strings to parse. The default is taken from sys.argv.
    #namespace - An object to take the attributes. The default is a new empty Namespace object.
    parser.parse_args()
        
handle_args_state()