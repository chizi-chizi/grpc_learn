#!/usr/bin/env python
# -*- coding: utf-8 -*-

from __future__ import print_function
import logging

import grpc
from  test_pb2 import Request, Response
from  test_pb2_grpc import TestStub 

def getFile(stub, request):
    contents= stub.GetFile(request)

    for content in contents:
        print("client receive %s" % (content.value))

    return

def run():
    # 使用with语法保证channel自动close
    with grpc.insecure_channel('localhost:50051') as channel:
        # 客户端通过stub来实现rpc通信
        stub = TestStub(channel)

        # 客户端必须使用定义好的类型，这里是HelloRequest类型, SayHello以及HelloRequest在proto文件中定义
        request = (request for request in [Request(file_path="./test.txt")])
        print(request)
        getFile(stub, request)

if __name__ == "__main__":
    logging.basicConfig()
    run()
