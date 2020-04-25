from time import sleep
from time import process_time
from time import time
import serial 
from serial_thread_ltc1660 import SerialHandler
import sys

# def read_sensor(serial_port, angle=[0, 0]): # angle: [max, min]

def loading(t, serial_port):
	time_end = time() + t
	print("reading from sensors...")
	while time() < time_end:
		print(" * ", end=''),
		sys.stdout.flush()
		sleep(0.2)
		data = serial_port.read()
	return


def read_sensor(serial_port): # angle: [max, min]
	# dlist = []
	# rcv_data = ''

	try:
		# t_end = time() + 3
		# print("test reading...")
		# while time() < t_end:
		# 	# print("test reading...")
		# 	data = serial_port.read() # Read the newest output from the Arduino
		# 	char = data.decode('utf-8')
		# 	if char == '\n':
		# 		data_list = rcv_data[:-1].split(',')
		# 		rcv_data = ''
		# 		data_list = list(map(int, data_list))
		# 		# print(data_list)
		# 	else:
		# 		rcv_data += char

		loading(3, serial_port)
		dlist = []
		rcv_data = ''
		while True:
			# print("test reading...")
			data = serial_port.read() # Read the newest output from the Arduino
			# sleep(0.005)
			char = data.decode('utf-8')
			if char == '\n':
				data_list = rcv_data[:-1].split(',')
				rcv_data = ''
				data_list = list(map(int, data_list))
				print(data_list)
				dlist.append(data_list)
			else:
				rcv_data += char

	except KeyboardInterrupt:
		print("\n")
		return dlist[1:]



def average(data):
	l = len(data)
	def divide(x): return x / l
	return map(divide, map(sum, zip(*data)))

def mapping(x, val_max, val_min, deg_max, deg_min):
	try:
		# max_deg to max_val situation
		return int(deg_min + (x - val_min) * (deg_max - deg_min) / (val_max - val_min))
		# min_deg to min_val situation
		# return int(deg_max - (x - val_min) * (deg_max - deg_min) / (val_max - val_min))
	except ZeroDivisionError:
		return 0

def deg_compensate(serial_port):
	zero_flag = input("please keep fingers at 0 degree and press K, quit press Q")
	if zero_flag == 'k' or zero_flag == 'K':
		bias = average(read_sensor(serial_port))
		return bias
	elif zero_flag == q or zero_flag == Q:
		pass

def take_data1(serial_port): # output sensor values
	start_flag = input("please make your pose and press K, quit press Q: ")
	if start_flag == 'k' or zero_flag == 'K':
		avg_data = average(read_sensor(serial_port))
		return avg_data
	elif zero_flag == q or zero_flag == Q:
		pass

def take_data2(serial_port, max_pose, min_pose): # output degrees
	start_flag = input("please make your pose and press K, quit press Q: ")
	if start_flag == 'k' or zero_flag == 'K':
		avg_data = average(read_sensor(serial_port, angle=[max_pose, min_pose]))
		return avg_data
	elif zero_flag == q or zero_flag == Q:
		pass





# if __name__ == '__main__':
# 	# Open Serial poit
# 	ser = serial.Serial('/dev/cu.SLAB_USBtoUART',9600) # Establish the connection on a specific port
# 	sleep(2)

# 	# # Data Collection and transfer test
# 	print("receiving sensor value...")
# 	sensor_data = read_sensor(ser)
# 	# print(sensor_data)
# 	print("please put your hand in horizontal position")
# 	min_pose = take_data1(ser)

# 	print("please grip your hand")
# 	max_pose = take_data1(ser)
# 	print(max_pose)
# 	print(min_pose)

# 	print("pose your finger to a degree")
# 	x = take_data2(ser, max_pose, min_pose)
# 	print(x)
# 	# print(mapping(x, max_pose, min_pose, 100, 0))








	

