#!/bin/bash
echo "-----------> write begin <---------------"
./write > test_c.pb
echo "-----------> write end <---------------"
echo -e "\n"
echo "-----------> read begin <---------------"
python read.py
echo "-----------> read end<---------------"
