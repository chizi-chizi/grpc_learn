all: python-read c-write

c-write:
	protoc-c -I=./ --c_out=. ./test_c.proto
	gcc -g -o write test_c.pb-c.c test_c.c -L ./  -lprotobuf-c
python-read:
	protoc -I=./ --python_out=./ ./test_c.proto
	python3 -m grpc_tools.protoc -I=./ --python_out=./ --grpc_python_out=./ ./test_c.proto
clean:
	rm  *pb*  write
