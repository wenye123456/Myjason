基于json-tutorial的学习笔记
# tutorial01
* 在宏定义多条语句时需加上`do{}while(0)`防止在`if`语句展开时出现错误 

# tutorial02
* 包含`float.h`后可以使用`LONG_LONG_MAX`,`FLT_MAX `,`DBL_MAX `等宏
* 在宏定义中使用#可以实现将其他类型转为字符串
* `errno.h`存在宏errno存储错误值，可以使用`stderror(errno)`查看错误
* strtod(https://en.cppreference.com/w/c/string/byte/strtof)
* `math.h`中定义了INFINITE，可以用于判断是否超过量程

# tutorial03
* 在windows下使用`_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);`检测内存泄漏 [更多](https://github.com/miloyip/json-tutorial/blob/master/tutorial03_answer/tutorial03_answer.md)
* 在SSE4.2指令集中，可以使用硬件优化效率 [更多](https://zhuanlan.zhihu.com/p/20037058)