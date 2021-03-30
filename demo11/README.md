# 概述
学习health check
# 测试
## 测试一:grpc server未启动， 使用健康监测
```
root@nxddos:~/chizi/grpc/get_netshield_filter/demo11# ./grpc_health_probe-linux-386  -addr=localhost:50051
timeout: failed to connect service "localhost:50051" within 1s
```
## 测试二:服务端打开了健康监测服务
- 步骤一：运行./greeter_server(打开了health check service, 代码中有`grpc::EnableDefaultHealthCheckService(true);`);
- 步骤二：执行命令: `./grpc_health_probe-linux-386  -addr=localhost:50051`
```
root@nxddos:~/chizi/grpc/get_netshield_filter/demo11# ./grpc_health_probe-linux-386  -addr=localhost:50051
status: SERVING
```
## 测试三:服务端未打开健康监测服务
- 步骤一：运行./greeter_async_server(代码中没有`grpc::EnableDefaultHealthCheckService(true)`, 即没有打开health check service);
- 步骤二：执行命令: `./grpc_health_probe-linux-386  -addr=localhost:50051`
```
root@nxddos:~/chizi/grpc/get_netshield_filter/demo11# ./grpc_health_probe-linux-386  -addr=localhost:50051
error: this server does not implement the grpc health protocol (grpc.health.v1.Health)
root@nxddos:~/chizi/grpc/get_netshield_filter/demo11#
```
# 参考
[grpc_health_probe-go实现](https://github.com/grpc-ecosystem/grpc-health-probe)
