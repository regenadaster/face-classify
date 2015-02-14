#!/usr/bin/env python
# -*-coding:utf8-*-

import os, sys
import socket
from module_manage import *
import threading
from img_predeal import *
import string
class cnnServer(object):
    
    def __init__(self, addr='10.2.15.11', sock=5678):
        self.addr = addr
        self.sock = sock
        self.module_manager = Manage()
        self.module_manager.engine_start()
        self.image_dir_dict = {}
        self.dict_lock = threading.Lock()

    def service_start(self):
        service_thread = threading.Thread(target=self.serviceProvider);
        service_thread.start()       
 

    def serviceProvider(self):
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.bind(('10.2.15.11', 5676))
        s.listen(5)
        while True:
            sock, addr = s.accept()
            t = threading.Thread(target=self.tcplink, args=(sock, addr))
            t.start()
    
    def process_image(self):
        origin_root = "/home/liugt/preprocess/save"
        dest_file   = "/home/liugt/test.pkl"

        image_size = (47,55,3) # width, height, channel
        rate = 0.11
        if not origin_root.endswith('/'):
            origin_root += '/'

        file_path_and_labels, map_label = data_shuffle(origin_root)
        train_files, valid_files = train_valid(file_path_and_labels, rate)
        print len(map_label)
        print len(file_path_and_labels)


        print 'make array ...'
        train_arr = make_array(train_files, image_size)
        print 'train_arr shape:', train_arr[0].shape, train_arr[1].shape
        valid_arr = make_array(valid_files, image_size)
        print 'valid_arr shape:', valid_arr[0].shape, valid_arr[1].shape
        arr_data = [train_arr, valid_arr]

        print 'dump files ...'
        dump2file(dest_file, arr_data)
        self.dict_lock.acquire()
        try:
            self.image_dir_dict = map_label
            print(self.image_dir_dict)       
        finally:
            self.dict_lock.release()
    def image_preprocess_thread(self):
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.bind(('10.2.15.11', 5679))
        s.listen(1)
        while True:
            sock, addr = s.accept()
            while True:
                data = sock.recv(1024)
                print(data)
                if data == 'exit' or not data:
                    break;
                self.process_image()
    
    def image_preprocess(self):
        process_thread = threading.Thread(target=self.image_preprocess_thread);
        process_thread.start()
    def tcplink(self, sock, addr):
        while True:
            data= sock.recv(1024)
            print(data)
            if data == 'exit' or not data:
                break
                
            result = self.module_manager.getPrediction(data)
            sock.send(self.image_dir_dict[string.atoi(result)])
        sock.close()    


if __name__ == '__main__':
    server = cnnServer()
    server.process_image()
    server.image_preprocess()
    
    server.service_start()



