#!/usr/bin/env python
# -*- coding: utf-8 -*-

from __future__ import print_function
import logging
from   health_check_pb2 import HealthCheckRequest
from health_check_pb2_grpc import HealthStub
import grpc

SERVICE_NAME = ["l3_grpc_service_health",
                "test", ""]


def run():

    with grpc.insecure_channel('localhost:50051') as channel:
        stub = HealthStub(channel)

        for service_name in SERVICE_NAME:
            request = HealthCheckRequest(service = service_name)
            #response = stub.Check(request, timeout=1)
            response = stub.Check(request, timeout =1000)
            print ("*"*50)
            print (response)

if __name__ == "__main__":
    logging.basicConfig()
    try:
        run()
    except grpc.RpcError as r:
        print (r)
