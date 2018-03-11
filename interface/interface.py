#!/usr/bin/env python
import sys
import getopt
import serial
import pyglet
import pygame
import time
import threading

pyglet.lib.load_library('avbin')
pyglet.have_avbin=True

song = None

def play_sound(file_name):
    global song
    if song is not None:
        song.stop()
    try:
        song = pygame.mixer.Sound(file_name)
        song.play()
    except pygame.error:
        pass


def play_ticker():
    play_sound('sounds/ticker.ogg')


def play_explosion():
    play_sound('sounds/boom.ogg')


def pong():
    print('pong')


def usage():
    print('Usage:\n\
  -h --help\tprint help\n\
  -p --port\tdevice name\n\
  -b --baudrate\tbaud rate')


def loop(ser):
    call = {
            'ping': pong,
            'tick': play_ticker,
            'explosion': play_explosion
    }
    pygame.init()
    while True:
        data = ser.readline().rstrip().decode()
        print('Receive :', data)
        try:
            call[data]()
        except KeyError:
            pass
    pygame.quit()


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
    print('Listening', path, 'at', baudrate, 'Bd.')
    ser = serial.Serial(path, baudrate)
    loop(ser)


if __name__ == '__main__':
    main(sys.argv)
