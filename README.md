# 概述
有关grpc的学习测试记录
# demo测试例介绍
- demo1: 测试grpc c++支持proto2
- demo2: 学习使用proto-c 封装protobuf消息
- demo3: 记录直接把c语言封装的pb格式转换成c++支持的格式时， server端运行会出现了段错误
- demo4: 抓包对比C封装后的消息结构，和grpc通信时的消息结构的差别
- demo4-2: 对比c语言对proto2和proto3的支持， 若使用proto3的时候， 使用c语言封装成的pb文件出现截断现象。
- demo5: 写c语言封装pb消息的接口， 该接口供c++代码调用的方式一
- demo6: 写c语言封装pb消息的接口， 该接口供c++代码调用的方式二,和方式一差不多,区别在于string类的使用
- demo7: 写c语言封装pb消息的接口， 该接口供c++代码调用的第三种方式， 使用bytes类型
- demo8: 修改helloworld实例， 学习异步接口, 并设置异步接口的超时时间
- demo9: 解决demo8出现的断言错误（未解决）
- demo10: 探究同步和异步的区别。 以及一些源码分析
- demo11: 探究同步服务器设置min_poll max_poll 线程池数量等的方法
- demo12: 学习health check
- demo13: 学习注册service_name的方法, 以及健康检查协议分析。
