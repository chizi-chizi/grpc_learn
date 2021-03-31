test:
	protoc-c -I=./ --c_out=. ./test.proto
	gcc -g -o test test.pb-c.c test.c -L ./  -lprotobuf-c
clean:
	rm -f *.o  test  *pb-c*
