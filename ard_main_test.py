# This is a script to open receiver port and send pre-processed data

import serial
from subprocess import Popen, PIPE
import sys
from readdata import serOpen, Norm, readCOM, time_keeper
import time



def main_read(subpipe, ser_rcv):

	output = subpipe.stdout.readline() # bytes

	print(repr(output))
	ser_rcv.write(output[:-1])
	time.sleep(1) # time delay in order to finish serial write

	# # print to console for receiving test
	# sys.stdout.write(ser_rcv.readline().decode('utf-8'))
	# sys.stdout.flush()



# new pipe for opening sender port
p_send = Popen(['python', './ard_sub_test.py'], stdin=PIPE, stdout=PIPE, stderr=PIPE)

port_num = '/dev/cu.usbmodem14401'
BAUDRATE = 250000
ser_rcv = serOpen('Receive_ser', port_num, BAUDRATE)

while True:
	main_read(p_send, ser_rcv)








