# This is a script to open sender port and process raw data

import serial
# from subprocess import Popen, PIPE
import sys
from readdata import serOpen, Norm, readCOM, time_keeper, reform, data_split, send_data
import time


port_num = '/dev/cu.usbmodem14601'
BAUDRATE = 250000

ser_send = serOpen('Send_ser', port_num, BAUDRATE)



while True:
	try:
		# read data from glove sensor
		data = ser_send.readline()
		data = data.decode('utf-8')

		# reformat to muscle input
		data_int = data_split(data)
		full_data = reform(data_int)
		data = ",".join([str(i) for i in full_data]) + "\0"
		
		# # write to main
		# print(repr(data)) # for reformat check
		sys.stdout.write(data+'\n')
		sys.stdout.flush()


	except KeyboardInterrupt:
		break




