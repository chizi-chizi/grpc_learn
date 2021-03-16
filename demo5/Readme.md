# 概述
写c语言封装pb消息的接口， 该接口供c++代码调用的方式一：
···
      string pbMsg;
      char* buf= NULL;   // 初始化为 null 的指针
      int len;
      buf = new char[128];

      getPbMsg(buf, &len);
      std::cout<<"len"<<len<<std::endl;
      pbMsg.assign(buf, 0, len);//使用string类的assign方法

      person.ParseFromString(pbMsg);
      stream->Write(person);
      /*
      person.set_name("chizi");
      person.set_age(100);
      person.set_weight(100);
      stream->Write(person);
···
# 测试步骤
- 步骤1.执行make命令
- 步骤2.执行 ./server
- 步骤3.执行python client.py
# 输出结果
当执行python client.py， 服务端将打印段错误信息，如下：
```
root@nxddos:~/demo5# python client.py
client receive :

name: "chizi"
age: 100
weight: 100
```
# 清理
```
make clean
```
