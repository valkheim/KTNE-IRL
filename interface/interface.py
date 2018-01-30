#!/usr/bin/env python
import sys
import getopt
import serial


def pong():
    print('pong')


def usage():
    print('Usage:\n\
  -h --help\tprint help\n\
  -p --port\tdevice name\n\
  -b --baudrate\tbaud rate')


def main(argv):
    try:
        opts, args = getopt.getopt(sys.argv[1:], "hb:p:",
                                   ["help", "path=", "baudrate="])
    except getopt.GetoptError as err:
        print(str(err))
        usage()
        sys.exit(2)
    path = '/dev/ttyACM0'
    baudrate = 9600
    for o, a in opts:
        if o in ("-h", "--help"):
            usage()
            sys.exit()
        elif o in ("-p", "--path"):
            path = a
        elif o in ("-b", "--baudrate"):
            baudrate = a
        else:
            assert False, "unhandled option"
    call = {
            'ping': pong,
            }
    print('Listening', path, 'at', baudrate, 'Bd.')
    ser = serial.Serial(path, baudrate)
    while True:
        data = ser.readline().rstrip().decode()
        call[data]()


if __name__ == '__main__':
    main(sys.argv)
