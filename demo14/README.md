# 概述
测试backoff重连机制. grpc客户端断线重连使用backoff机制, 重连间隔越来越长, 可以通过参数指定相关参数

相关参数:https://grpc.github.io/grpc/core/group__grpc__arg__keys.html#gad7d9d143858d8f5e138cf704b0082973
可参考 grpc/test/cpp/interop/reconnect_interop_client.cc
# backoff重连使用的api(未验证通过)
```
::grpc_impl::ChannelArguments args;
args.SetInt(GRPC_ARG_MAX_RECONNECT_BACKOFF_MS, 100);
args.SetInt(GRPC_ARG_MIN_RECONNECT_BACKOFF_MS, 100);
args.SetInt(GRPC_ARG_INITIAL_RECONNECT_BACKOFF_MS, 100);
std::shared_ptr<grpc_impl::Channel> channel = grpc::CreateCustomChannel(car_ip + ":" + GRPC_NAVIGATE_API_PORT, grpc::InsecureChannelCredentials(), args);
```
# 测试客户端与服务端断开后，默认情况下，客户端可以重连上服务端
- 第一步： 在终端1执行命令，启动客户端， 此时没有启动服务端

  ```
  root@nxddos:~/demo14# ./greeter_client
  message bridge service not available: 2130time
  14: failed to connect to all addresses
  message bridge service not available: 2131time
  14: failed to connect to all addresses
  message bridge service not available: 2132time
  14: failed to connect to all addresses
  message bridge service not available: 2133time
  14: failed to connect to all addresses
  message bridge service not available: 2134time
  ```

- 第二步：在终端2，启动服务端， 执行命令

  ```
  ./greeter_server  -i 1 -a 2 -t 10
  ```

  

- 第三步：观察终端1上， 可以连接上服务端

  ```
  message bridge service not available: 2131time
  14: failed to connect to all addresses
  message bridge service not available: 2132time
  14: failed to connect to all addresses
  message bridge service not available: 2133time
  14: failed to connect to all addresses
  message bridge service not available: 2134time
  Greeter received: Hello world
  Greeter received: Hello world
  Greeter received: Hello world
  ```

  

上面的数字代表执行的秒数。
# 参考
https://blog.csdn.net/tang05505622334/article/details/99759788
