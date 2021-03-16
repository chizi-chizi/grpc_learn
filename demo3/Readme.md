# 概述
记录直接把c语言封装的pb格式转换成c++支持的格式时， server端运行会出现了段错误
# 测试步骤
- 步骤1.执行make命令
- 步骤2.执行 ./server
- 步骤3.执行python client.py
# 输出结果
当执行python client.py， 服务端将打印段错误信息，如下：
```
root@nxddos:~/demo3# ./server
Server listening on 0.0.0.0:50051
Segmentation fault (core dumped)
```
# 清理
```
make clean
```
