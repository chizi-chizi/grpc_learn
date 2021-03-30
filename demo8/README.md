# 概述
修改helloworld实例， 学习异步接口:
- 修改前， 使用cq_-\>Next()接口获取事件， 此接口最终会调用epoll_wait()， 且超时时间设置为-1， 为长阻塞的。
- 修改后， 使用cq_-\>AsyncNext()接口获取事件，此时可以设置超时时间， 本例中修改为超时时间为1s。此外修改后，增加了相应SIGINT的功能。 
> 说明：修改后的代码仍然存在问题， 当CTRL+C的时候，会出现断言错误， 错误信息见下面章节
# 测试步骤
- 步骤1.执行make命令
- 步骤2.执行 ./greeter_async_server
- 步骤3.执行CTRL+C
# 输出结果
执行完步骤3后， 服务端将打印如下错误，如下：
```
root@nxddos:~/demo8# ./greeter_async_server
Server listening on 0.0.0.0:50051
-----begin----CallData
----status:Create
...............while..........
status:2
ok:0
timeout continue..
...............while..........
status:2
ok:0
timeout continue..
...............while..........
^Creceive:2
status:2
ok:0
timeout continue..
E0316 10:55:39.433627542   22042 completion_queue.cc:248]    assertion failed: queue.num_items() == 0
Aborted (core dumped)
```
# 清理
```
make clean
```
