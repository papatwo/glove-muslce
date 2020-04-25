import serial
import threading
import sys
import time
import os
import csv
import argparse
from collections import namedtuple


# Constants for fingers
THUMB, INDEX, MIDDLE, RING, LITTLE = 0, 1, 2, 3, 4
FINGERS = (THUMB, INDEX, MIDDLE, RING, LITTLE)
BEND_TIP, BEND_ROOT, EXTEND_TIP, EXTEND_ROOT = 0, 1, 2, 3
PARTS = (BEND_TIP, BEND_ROOT, EXTEND_TIP, EXTEND_ROOT)

# Constants for status of hand
RELAX, BEND, EXTEND, HOLD = 0, 1, 2, 3
STATUSES = (RELAX, BEND, EXTEND, HOLD)

# Constants for air pressure
MAX, THIRD, SECOND, FIRST, EMPTY = 1023, int(
    1023 / 4 * 3), int(1023 / 4 * 2), int(1023 / 4 * 1), 0
# MAX, SEVENTH, SIXTH, FIFTH, FOURTH, THIRD, SECOND, FIRST, EMPTY = 1023, int(1023/8*7), int(1023/8*6), int(1023/8*5), int(1023/8*4), int(1023/8*3), int(1023/8*2), int(1023/8*1), 0
PRESS_STATUS = (MAX, THIRD, SECOND, FIRST, EMPTY)
# PRESS_STATUS = (MAX, SEVENTH, SIXTH, FIFTH, FOURTH, THIRD, SECOND, FIRST, EMPTY)
preset_pressure = [[0] * len(PARTS) for i in range(len(STATUSES))]
preset_pressure[RELAX] = [EMPTY, EMPTY, EMPTY, EMPTY]
preset_pressure[BEND] = [MAX, MAX, EMPTY, EMPTY]
preset_pressure[EXTEND] = [EMPTY, EMPTY, MAX, MAX]
preset_pressure[HOLD] = [MAX, MAX, MAX, MAX]

# FILE_PREFIX = './csv_record/'
# FILE_PREFIX = './csv_perform/'
FILE_PREFIX = './pirot_experiment/'

CTRL_C = 3
BAUDRATE = 38400


def getch():
    import tty
    import termios
    fd = sys.stdin.fileno()
    old = termios.tcgetattr(fd)
    try:
        tty.setraw(fd)
        return sys.stdin.read(1)
    finally:
        termios.tcsetattr(fd, termios.TCSADRAIN, old)

def format_print(list_object, count=0):
    sys.stdout.write("\rNum{:3d}: ".format(count))
    for i in range(len(list_object)-1):
        print('{:4}, '.format(list_object[i]), end='')
    print('{:4}'.format(list_object[-1]))
    sys.stdout.flush()


class SerialHandler():

    def __init__(self):
        self.pressure = [EMPTY for i in range(20)]
        self.hand_status = [RELAX] * len(FINGERS)
        self.csv_info = {}
        self.tip_press_strength = 0
        self.root_press_strength = 0
        self.wrt_t = None
        try:
            self.ser = serial.Serial()
            self.ser.baudrate = BAUDRATE
            for file in os.listdir('/dev'):
                if "cu.usbmodem" in file:
                    self.ser.port = '/dev/' + file
            self.ser.open()
        except:
            print("No usb device found.")
            exit()

    def _thread2(self):
        while True:
            data = self.ser.read()
            sys.stdout.write(data.decode('utf-8'))

    def start_receiving(self):
        self.rcv_t = threading.Thread(target=self._thread2)
        self.rcv_t.daemon = True
        self.rcv_t.start()

    def _writer_thread(self, write_file, write_interval):
        with open(write_file, 'w') as f:
            writer = csv.writer(f)
            header = [write_interval, len(
                PRESS_STATUS)] + [state for state in PRESS_STATUS]
            writer.writerow(header)
            while self.isWriting:
                writer.writerow(self.pressure)
                time.sleep(write_interval)

    def start_writing(self, write_path, interval):
        self.isWriting = True
        self.wrt_t = threading.Thread(
            target=self._writer_thread, args=(write_path, interval))
        self.wrt_t.daemon = True
        self.wrt_t.start()

    def start_send_loop(self):
        print("Press <Enter> to exit.")
        print("Wait a moment for initializing......")
        self.send_values([0 for i in range(20)])
        time.sleep(1.5)
        print('OK.')
        while(True):
            # try:
            key = getch()
            if ord(key) == ord('p'):
                self.send_values([0 for i in range(20)])
                self.rcv_t.isRunning = False
                if self.wrt_t:
                    self.isWriting = False
                    time.sleep(1)
                    print("stopped")
                sys.exit(0)
            self.command(key)
            pressure = self.calc_pressure()
            self.send_values(pressure)
            print(pressure)
            # except:
            #     print("\nstop thread2")
            #     self.rcv_t.isRunning = False
            #     sys.exit(0)

    def send_values(self, pressures):
        nums = ",".join([str(power) for power in pressures]) + "\0"
        print('nums', nums)
        self.ser.write(bytes(nums, 'utf-8'))

    def command(self, chr):
        if chr == 'a':
            self._change_state(THUMB, BEND)
        elif chr == 's':
            self._change_state(INDEX, BEND)
        elif chr == 'd':
            self._change_state(MIDDLE, BEND)
        elif chr == 'f':
            self._change_state(RING, BEND)
        elif chr == 'g':
            self._change_state(LITTLE, BEND)

        if chr == 'q':
            self._change_state(THUMB, EXTEND)
        elif chr == 'w':
            self._change_state(INDEX, EXTEND)
        elif chr == 'e':
            self._change_state(MIDDLE, EXTEND)
        elif chr == 'r':
            self._change_state(RING, EXTEND)
        elif chr == 't':
            self._change_state(LITTLE, EXTEND)

        if chr == 'z':
            self._change_state(THUMB, HOLD)
        elif chr == 'x':
            self._change_state(INDEX, HOLD)
        elif chr == 'c':
            self._change_state(MIDDLE, HOLD)
        elif chr == 'v':
            self._change_state(RING, HOLD)
        elif chr == 'b':
            self._change_state(LITTLE, HOLD)

        if chr == '1':
            self._change_state(THUMB, RELAX)
        elif chr == '2':
            self._change_state(INDEX, RELAX)
        elif chr == '3':
            self._change_state(MIDDLE, RELAX)
        elif chr == '4':
            self._change_state(RING, RELAX)
        elif chr == '5':
            self._change_state(LITTLE, RELAX)

        if chr == '6':
            if self.hand_status[THUMB] == BEND:
                self._change_state(THUMB, EXTEND)
            else:
                self._change_state(THUMB, BEND)
        elif chr == '7':
            if self.hand_status[INDEX] == BEND:
                self._change_state(INDEX, EXTEND)
            else:
                self._change_state(INDEX, BEND)
        elif chr == '8':
            if self.hand_status[MIDDLE] == BEND:
                self._change_state(MIDDLE, EXTEND)
            else:
                self._change_state(MIDDLE, BEND)
        elif chr == '9':
            if self.hand_status[RING] == BEND:
                self._change_state(RING, EXTEND)
            else:
                self._change_state(RING, BEND)
        elif chr == '0':
            if self.hand_status[LITTLE] == BEND:
                self._change_state(LITTLE, EXTEND)
            else:
                self._change_state(LITTLE, BEND)

        if chr == '+':
            if self.tip_press_strength > 0:
                self.tip_press_strength -= 1
            if self.root_press_strength > 0:
                self.root_press_strength -= 1
            self.set_pressure_strength()

        elif chr == '-':
            if self.tip_press_strength < len(PRESS_STATUS) - 1:
                self.tip_press_strength += 1
            if self.root_press_strength < len(PRESS_STATUS) - 1:
                self.root_press_strength += 1
            self.set_pressure_strength()

        if chr == 'i':
            if self.tip_press_strength > 0:
                self.tip_press_strength -= 1
            self.set_pressure_strength()
        elif chr == 'o':
            if self.root_press_strength > 0:
                self.root_press_strength -= 1
            self.set_pressure_strength()
        elif chr == ',':
            if self.tip_press_strength < len(PRESS_STATUS) - 1:
                self.tip_press_strength += 1
            self.set_pressure_strength()
        elif chr == '.':
            if self.root_press_strength < len(PRESS_STATUS) - 1:
                self.root_press_strength += 1
            self.set_pressure_strength()

        if chr == '`':
            for FINGER in FINGERS:
                self._change_state(FINGER, RELAX)

        # print("status: ", [['RELAX', 'BEND', 'EXTEND', 'HOLD'][status] for status in self.hand_status])
        print("status: ", self.hand_status)

    def set_pressure_strength(self):
        tip = self.tip_press_strength
        root = self.root_press_strength
        preset_pressure[BEND] = [PRESS_STATUS[
            tip], PRESS_STATUS[root], EMPTY, EMPTY]
        preset_pressure[EXTEND] = [EMPTY, EMPTY,
                                   PRESS_STATUS[tip], PRESS_STATUS[root]]
        preset_pressure[HOLD] = [PRESS_STATUS[tip], PRESS_STATUS[
            root], PRESS_STATUS[tip], PRESS_STATUS[root]]
        print(PRESS_STATUS[tip], PRESS_STATUS[root])
        print(preset_pressure)

    def _change_state(self, finger, status):
        if self.hand_status[finger] == status:
            self.hand_status[finger] = RELAX
        else:
            self.hand_status[finger] = status

    def calc_pressure(self):
        for finger in FINGERS:
            self.pressure[finger * 4: finger * 4 +
                          4] = preset_pressure[self.hand_status[finger]]
        return self.pressure

    def start_play_csv(self, path, repeat=1, sleep_time=None, time_interval=0.2):
        start_time = time.time()
        try:
            for i in range(repeat):
                loop_start_t = time.time()
                with open(path, 'r') as f:
                    reader = csv.reader(f)
                    header = next(reader)
                    self.set_csv_info(header)
                    key_mapping = next(reader)
                    pressure = self.csv_info['pressure']
                    if time_interval == 0.2:
                        time_interval = self.csv_info['time_interval']
                    else:
                        self.csv_info['time_interval'] = time_interval
                    print('csv_info', self.csv_info)
                    self.send_values([0 for i in range(20)])
                    if i == 0:
                        print('sent')
                        time.sleep(1.5)

                    start_sending_time = time.time()
                    for i, row in enumerate(reader):
                        values = list(map(int, row))
                        # pressure_values = [pressure[num] for num in values]
                        # self.send_values(pressure_values)
                        # print(pressure_values)
                        self.send_values(values)
                        format_print(values, count=i)
                        time.sleep(time_interval)
                    sending_elapsed_t = time.time() - start_sending_time
                    print(sending_elapsed_t)
                loop_elapsed_t = time.time() - loop_start_t
                # print(loop_elapsed_t)
                if sleep_time is not None:
                    time.sleep(sleep_time)
                print('file end.')
            print('End.')
        except KeyboardInterrupt:
            print('End.')
        finally:
            total_elapsed_t = time.time() - start_time
            print(total_elapsed_t)
            self.send_values([0] * 20)
            time.sleep(0.1)

    def set_csv_info(self, header):
        self.csv_info['time_interval'] = float(header[0])
        num_of_values = int(header[1])
        self.csv_info['pressure'] = list(map(int, header[2:2 + num_of_values]))


def main():
    parser = argparse.ArgumentParser(
        description='Determine whether keyboard or play mode.')
    parser.add_argument('-p', '--play', dest='play_csv_file',
                        type=str, default=None, help='Play recorded csv file')
    parser.add_argument('-r', '--record', dest='write_csv_file',
                        type=str, default=None, help='Record input history to csv file')
    parser.add_argument('-t', '--interval', type=float, default=0.2)
    parser.add_argument('-w', '--weaken', type=int, default=0)
    parser.add_argument('-u', '--repeat', type=int, default=1)
    parser.add_argument('-s', '--sleeptime', type=float, default=None)

    args = parser.parse_args()
    ser_handler = SerialHandler()
    ser_handler.start_receiving()
    if args.weaken:
        if args.weaken < len(PRESS_STATUS):
            ser_handler.tip_press_strength += args.weaken
            ser_handler.root_press_strength += args.weaken
            ser_handler.set_pressure_strength()
        else:
            ser_handler.tip_press_strength = len(PRESS_STATUS) - 1
            ser_handler.root_press_strength = len(PRESS_STATUS) - 1
            ser_handler.set_pressure_strength()
    if args.play_csv_file:
        ser_handler.start_play_csv(
            FILE_PREFIX + args.play_csv_file, args.repeat, args.sleeptime, time_interval=args.interval)
    else:
        if args.write_csv_file:
            ser_handler.start_writing(
                FILE_PREFIX + args.write_csv_file, interval=args.interval)
        ser_handler.start_send_loop()


if __name__ == '__main__':
    main()
