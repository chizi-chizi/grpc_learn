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
