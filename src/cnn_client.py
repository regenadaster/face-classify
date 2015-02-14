#!/usr/bin/env python
# -*- coding:utf8 -*-

import socket

def getService():
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect(('10.2.15.11',5676))
    while True:
    	request = raw_input()
    	s.send(request)
    	d = s.recv(1024)

    	print(d)

if __name__ == '__main__':
	getService()

