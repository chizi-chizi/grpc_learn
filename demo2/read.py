#!/usr/bin/env python
# -*- coding: utf-8 -*-

from __future__ import print_function

import grpc
from  test_c_pb2 import  Person


if __name__ == "__main__":
    person = Person()
    with open("./test_c.pb", "rb") as f:
        person.ParseFromString(f.read())
        print (person)
