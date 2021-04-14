# 参考
[how to use health check](https://github.com/grpc/grpc/issues/13962)
[GRPC Health Checking Protocol](https://github.com/grpc/grpc/blob/master/doc/health-checking.md)
# 概述
学习注册service_name的方法,使用如下函数：
```c++
//grpc/src/cpp/server/health/default_health_check_service.cc 设置回调函数。
void DefaultHealthCheckService::RegisterCallHandler(
    const std::string& service_name,
    std::shared_ptr<HealthCheckServiceImpl::CallHandler> handler)

void DefaultHealthCheckService::UnregisterCallHandler(
    const std::string& service_name,
    const std::shared_ptr<HealthCheckServiceImpl::CallHandler>& handler)

//grpc/src/cpp/server/health/default_health_check_service.cc 设置health check的service name
DefaultHealthCheckService::DefaultHealthCheckService() {
  services_map_[""].SetServingStatus(SERVING);
}

void DefaultHealthCheckService::SetServingStatus(
    const std::string& service_name, bool serving) {
  grpc_core::MutexLock lock(&mu_);
  if (shutdown_) {
    // Set to NOT_SERVING in case service_name is not in the map.
    serving = false;
  }
  services_map_[service_name].SetServingStatus(serving ? SERVING : NOT_SERVING);
}


//grpc/include/grpcpp/server.h  或者server的healtch check server
 HealthCheckServiceInterface* GetHealthCheckService() 
```
# 修改代码
```
using grpc::HealthCheckServiceInterface;

HealthCheckServiceInterface* health_server = server->GetHealthCheckService();
//可以设置多次
health_server->SetServingStatus(service_name, true);
health_server->SetServingStatus(test_name, true);
```
# 测试
- 第一步：编译：make
- 第二步：运行server: ./greeter_server
- 第三步：运行grpc_health_check.py
结果为：
```
\root@nxddos:~/demo13# python grpc_health_check.py
**************************************************
status: SERVING

**************************************************
status: SERVING

**************************************************
status: SERVING
```
