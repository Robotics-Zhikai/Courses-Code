## 分布比较杂的知识点总结
* [指针与引用的区别](https://blog.csdn.net/qq_27678917/article/details/70224813)
* [堆和栈的区别](https://zhuanlan.zhihu.com/p/66922957)
* [详解C/C++中volatile关键字](https://blog.csdn.net/weixin_44363885/article/details/92838607)
	* 不把变量缓存在寄存器中，每次都要从内存中读取新鲜的数据 
* [C++中为什么构造函数不能为虚函数](https://blog.csdn.net/qq_34673519/article/details/101438884)
	* 构造函数不能定义为虚函数。在构造函数中可以调用虚函数，不过此时调用的是正在构造的类中的虚函数，而不是子类的虚函数，因为此时子类尚未构造好。
* [有名管道 FIFO](https://blog.csdn.net/qq_22075977/article/details/77832748)
	* Pipe 和 FIFO 的管道数据都存在内核内存的缓冲区中
	* 有名管道 FIFO 和无名管道 Pipe 主要的区别就是 FIFO 在磁盘上建立管道文件（FIFO 将内核数据缓冲区映射到了实际的文件节点），所以我们可以在磁盘上实际看到，故称为「有名字」，而 Pipe 没有在磁盘上建立文件，我们不能实际看到，故称为「无名」，其实就这么简单的理解。
	* FIFO在网络编程中使用不多（Linux高性能编程有讲述）
* [Linux进程间通信 System V标准&POSIX标准](https://blog.csdn.net/xiaoting451292510/article/details/103819933?utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7EBlogCommendFromMachineLearnPai2%7Edefault-5.base&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7EBlogCommendFromMachineLearnPai2%7Edefault-5.base)
	*  System V：System V更多用于进程间通信，线程间使用较少。线程相对于进程是轻量级的，例如调度的策略开销，如果使用System V这种每次调用都会陷入内核的接口，会丧失线程的轻量优势。所以，多线程之间的通信不使用System V的接口函数
	*  POSIX：POSIX进程间&线程间通信同步更优。