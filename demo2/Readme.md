
# 概述
学习使用proto-c 封装protobuf消息
# 依赖
安装protoc-c，略
# 测试步骤
- 步骤1.执行make命令
- 步骤2.执行test.sh 脚本
# 输出结果
```
-----------> write begin <---------------
Writing 11 serialized bytes
-----------> write end <---------------


-----------> read begin <---------------
name: "chizi"
age: 100
weight: 100

-----------> read end<---------------
```
# 清理
```
make clean
```
