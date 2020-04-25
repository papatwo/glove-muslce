import serial
import threading
import os
import time
import sys
from calibration2 import calibration
from serial_thread_ltc1660 import SerialHandler
# from sensor import BendSensor, PressureSensor


def serOpen(ser_type, port_num, bdrate):
    ser = serial.Serial()
    ser.baudrate = bdrate
    ser.port = port_num
    ser.open()
    # print(ser_type, ser.isOpen()) # ser_type = ser_read/ser_write

    return ser



def time_keeper(n):
    time_end = time.time() + n
    while time.time() < time_end:
        print(" * ", end=''),
        sys.stdout.flush()
        time.sleep(0.2)
    return



def Norm(ser):
    norm_max, norm_min = calibration(ser)
    # print('norm is: ', norm_max, norm_min)
    norm_max = list(map(lambda x: x[0]-x[1], zip(norm_max, norm_min)))
    print('Norm_max is', norm_max)
    print('Norm_min is', norm_min)
    
    return [norm_max, norm_min]



def reform(glove):
    muscle = [0] * 40
    # Thumb tip sensor reading
    if glove[0] > 0: # flex
        muscle[0] = glove[0]
    else: # extension
        muscle[2] = glove[0]
    # Thumb root sensor reading
    if glove[1] > 0: 
        muscle[1] = glove[1]
    else:
        muscle[3] = glove[1]


    # Index tip sensor reading
    if glove[2] > 0: # flex
        muscle[8] = glove[2]
    else: # extension
        muscle[10] = glove[2]
    # Index root sensor reading
    if glove[2] > 0: 
        muscle[9] = glove[4]
    else:
        muscle[11] = glove[4]


    # Middle tip sensor reading
    if glove[5] > 0: # flex
        muscle[16] = glove[5]
    else: # extension
        muscle[18] = glove[5]
    # Middle root sensor reading
    if glove[7] > 0: 
        muscle[17] = glove[7]
    else:
        muscle[19] = glove[7]


    # Ring tip sensor reading
    if glove[8] > 0: # flex
        muscle[24] = glove[8]
    else: # extension
        muscle[26] = glove[8]
    # Ring root sensor reading
    if glove[10] > 0: 
        muscle[25] = glove[10]
    else:
        muscle[27] = glove[10]


    # Small tip sensor reading
    if glove[11] > 0: # flex
        muscle[32] = glove[11]
    else: # extension
        muscle[34] = glove[11]
    # Small root sensor reading
    if glove[13] > 0: 
        muscle[33] = glove[13]
    else:
        muscle[35] = glove[13]

    return muscle



def data_split(data):
    d_split = data[:-1].split(',')
    data_int = list(map(int, d_split))
    return data_int



def send_data(data):
    nums = ",".join([str(i) for i in data]) + "\n"
    # print('nums', nums)
    return nums



def readCOM(ser, norm_set):
    dlist=[]
    rcv_data = ''
    norm_max = norm_set[0]
    norm_min = norm_set[1]

    while True:
        try:
            data = ser.read() # read output
            char = data.decode('utf-8')

            if char == '\n':
                data_list = rcv_data[:-1].split(',')
                # print(repr(rcv_data))
                rcv_data = ''
                # if data_list[0] is not 'Ready':
                data_list = list(map(int, data_list))
                dlist.append(data_list)
                # print(data_list)
                pose = list(map(lambda x: x[0]-x[1], zip(data_list, norm_min)))
                norm_pose = list(map(lambda x: x[0]/x[1], zip(pose, norm_max)))
                print('normed', norm_pose[1])
                glove = list(map(lambda x: round(1024*x), norm_pose))

                muscle_sig = reform(glove)
                # print('normed', muscle_sig)
                
                sys.stdout.flush()
                time.sleep(0.1)

            else:
                rcv_data += char
        except KeyboardInterrupt:
            # return muscle_sig
            break







