from sensor_sercom2 import read_sensor, average, mapping, deg_compensate, take_data1, take_data2
from time import sleep
from time import process_time
from time import time
import serial 

def calibration(ser):
	##### Calibration #####

	# Open Serial poit
	# ser = serial.Serial('/dev/cu.usbmodem144401',250000) # Establish the connection on a specific port
	# sleep(2)

	# # Data Collection and transfer test
	print("receiving sensor value...")
	sensor_data = read_sensor(ser)
	print(list(average(sensor_data)))
	# print(sensor_data)
	print("please put your hand in horizontal position")
	min_pose = list(take_data1(ser))

	print("please grip your hand")
	max_pose = list(take_data1(ser))
	# print('max',list(max_pose))
	# print('min',list(min_pose))

	return max_pose, min_pose

	