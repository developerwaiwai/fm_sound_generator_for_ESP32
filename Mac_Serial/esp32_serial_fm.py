from ctypes import *
import serial
from struct import *
from multiprocessing import Process, connection, current_process

import csv
from oto_data import *

from time import sleep

def make_serial_data_param(command, amp100, mul, attack, decay, sus_level100, release_level100):
    return pack("H", command) + pack("H", amp100) + pack("H", mul) + pack("I", 0) + pack("Q", attack) + pack("Q", decay) + pack("H", sus_level100) + pack("H", release_level100) + pack("I", 0)


def make_serial_data_note(command, on_off, algorism, tempo, helz, note_type):
    return pack('H', command) + pack('H', on_off) + pack('H', algorism) + pack('H', tempo) + pack('H', helz) + pack('Q', note_type) + pack('Q', 0) + pack('Q', 0)


def send_algorism_param(serial, param_num, amp100, mul, attack, decay, sus_level100, release_level100):
    param_data = make_serial_data_param(param_num, amp100, mul, attack, decay, sus_level100, release_level100)
    serial.write(param_data)
    serial.read()    #return ACK


def send_note(serial, on_off, algorism_num, tempo, helz, notetype):
    note_data = make_serial_data_note(0x10, on_off, algorism_num, tempo, helz, notetype)
    serial.write(note_data)
    serial.read()    #return ACK


def read_and_send_sound_param(serial, file_name):
    f = open(file_name, "r")
    d = csv.DictReader(f)
    index = 1
    for row in d:
        if row["amp100"] is "":
            break
        send_algorism_param(serial, index, int(row["amp100"]), int(row["multiple"]), int(row["attack"]), int(row["decay"]), int(row["sus_level100"]), int(row["rel_level100"]))
        index = index + 1
    f.close()


def read_and_send_note(serial, file_name):
    f = open(file_name, "r")
    d = csv.DictReader(f)
    for row in d:
        if row["on_off"] is "":
            break;
        send_note(serial, int(row["on_off"]), int(row["algorism"]), int(row["tempo"]), int(ONKAI[row["helz"]]), int(ONPU[row["length"]]))
    f.close()



def fm_sound_transfer(serial_port, sound_color_file, note_file):

    print serial_port + sound_color_file + note_file
    s = serial.Serial(serial_port, 115200)
    s.timeout = 30.0
    read_and_send_sound_param(s, sound_color_file)
    read_and_send_note(s, note_file)

    s.close()

pool = []
settings_filename = "settings.csv"
f = open(settings_filename)
data = csv.reader(f, delimiter=",", doublequote=True, lineterminator="\n", quotechar='"', skipinitialspace=True)
for row in data:
    if row[0] is "":
        break

    port = row[0]
    sound_color = row[1]
    note = row[2]

    pool.append(Process(target=fm_sound_transfer, args=(port, sound_color, note)))


for p in pool:
    p.start()

sleep(1.0)

for p in pool:
    p.join()


f.close()











# param_data = make_serial_data_param(1, 100, 1, 300, 1000000, 0, 0)
# s.write(param_data)
# s.read()
#
# param_data = make_serial_data_param(2, 50, 2, 300, 1000000, 0, 0)
# s.write(param_data)
# s.read()
#
# param_data = make_serial_data_param(3, 300, 3, 300, 1000000, 0, 0)
# s.write(param_data)
# s.read()
#
# param_data = make_serial_data_param(4, 100, 4, 300, 1000000, 0, 0)
# s.write(param_data)
# s.read()

# note_data = make_serial_data_note(0x10, 1, 7, 120, 392, 500000)
# s.write(note_data)
# s.read()
#
# note_data = make_serial_data_note(0x10, 1, 7, 120, 330, 500000)
# s.write(note_data)
# s.read()
#
# note_data = make_serial_data_note(0x10, 1, 7, 120, 330, 1000000)
# s.write(note_data)
# s.read()
#
# note_data = make_serial_data_note(0x10, 1, 7, 120, 349, 500000)
# s.write(note_data)
# s.read()
#
# note_data = make_serial_data_note(0x10, 1, 7, 120, 294, 500000)
# s.write(note_data)
# s.read()
#
# note_data = make_serial_data_note(0x10, 1, 7, 120, 294, 1000000)
# s.write(note_data)
# s.read()
#
#
# note_data = make_serial_data_note(0x10, 1, 7, 120, 262, 500000)
# s.write(note_data)
# s.read()
# note_data = make_serial_data_note(0x10, 1, 7, 120, 294, 500000)
# s.write(note_data)
# s.read()
# note_data = make_serial_data_note(0x10, 1, 7, 120, 330, 500000)
# s.write(note_data)
# s.read()
# note_data = make_serial_data_note(0x10, 1, 7, 120, 349, 500000)
# s.write(note_data)
# s.read()
# note_data = make_serial_data_note(0x10, 1, 7, 120, 392, 500000)
# s.write(note_data)
# s.read()
# note_data = make_serial_data_note(0x10, 1, 7, 120, 392, 500000)
# s.write(note_data)
# s.read()
# note_data = make_serial_data_note(0x10, 1, 7, 120, 392, 1000000)
# s.write(note_data)
# s.read()
