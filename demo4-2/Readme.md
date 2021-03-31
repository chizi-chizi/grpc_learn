# 概述
对比c语言对proto2和proto3的支持， 若使用proto3的时候， 使用c语言封装成的pb文件出现截断现象。
# 测试环境
```
root@nxddos:~/demo4-2# protoc-c --version
protobuf-c 1.2.1
libprotoc 3.0.0
```
# 测试步骤
- 步骤1.执行make命令
- 步骤2.执行 ./test
- 步骤3.执行demo4中的（demo4中使用的是proto2语法）./test


# 输出结果
当执行步骤2，输出结果如下：
```
root@nxddos:~/demo4# ./test
len:11
a 5 63 68 69 7a 69 10 64 18 64
```
当执行步骤3，输出结果如下：
```
root@nxddos:~/demo4# ./test
len:7
a 5 63 68 69 7a 69
```

# 结论
可以看出c语言，对proto3语法的支持存在一些问题。 




