# 参考
- [how to use health check](https://github.com/grpc/grpc/issues/13962)
- [GRPC Health Checking Protocol](https://github.com/grpc/grpc/blob/master/doc/health-checking.md)
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
# health check proto学习
## 客户端
1. 客户端在获取服务端的健康状态的时候，必须设置超时时间， 如果在超时时间内， 健康监测还未完成， 客户端应该声明服务端是unhealth的
2. 客户端需能处理服务端没有运行health service的情况。 
## 服务端
1. 服务端应该使用一个空的字符串作为健康状态的服务名，为了兼容客户端对服务名不感兴趣，此时也能进行健康监测。
2. 服务端应该手头设置所以的可能服务。如果服务名被找到， 应该返回OK， 但里面的字段可以是SERVING NOT SERVING , 如果服务名没有被找到， 应该返回NOT_FOUND

下面的源码中， 可以看到， 在c++的 default health check实现中， 使用了空字符串作为服务名：
```c++
//cpp/server/health/default_health_check_service.cc
DefaultHealthCheckService::DefaultHealthCheckService() {
  services_map_[""].SetServingStatus(SERVING);
}

```
下面的源码中， 可以看出监控检测时， 对返回值的处理
```
//grpc/src/cpp/server/health/default_health_check_service.cc

bool DefaultHealthCheckService::HealthCheckServiceImpl::EncodeResponse(
    ServingStatus status, ByteBuffer* response) {
  upb::Arena arena;
  grpc_health_v1_HealthCheckResponse* response_struct =
      grpc_health_v1_HealthCheckResponse_new(arena.ptr());
  grpc_health_v1_HealthCheckResponse_set_status(
      response_struct,
      status == NOT_FOUND
          ? grpc_health_v1_HealthCheckResponse_SERVICE_UNKNOWN
          : status == SERVING ? grpc_health_v1_HealthCheckResponse_SERVING
                              : grpc_health_v1_HealthCheckResponse_NOT_SERVING);
  size_t buf_length;
  char* buf = grpc_health_v1_HealthCheckResponse_serialize(
      response_struct, arena.ptr(), &buf_length);
  if (buf == nullptr) {
    return false;
  }
  grpc_slice response_slice = grpc_slice_from_copied_buffer(buf, buf_length);
  Slice encoded_response(response_slice, Slice::STEAL_REF);
  ByteBuffer response_buffer(&encoded_response, 1);
  response->Swap(&response_buffer);
  return true;
}

```

