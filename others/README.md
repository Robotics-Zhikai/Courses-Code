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
* [多线程还是多进程，二者的选择问题（其中涉及到线程栈）](https://blog.csdn.net/lishenglong666/article/details/8557215)
	* 下面的实验打印次数不变，而进程/线程数量逐渐增加。在实验过程中多线程程序在后四组（线程数350，500，800,1000）的测试中都出现了“段错误”，出现错误的原因和多线程预分配线程栈有关。实验中的计算机CPU是32位，寻址最大范围是4GB（2的32次方），Linux是按照3GB/1GB的方式来分配内存，其中1GB属于所有进程共享的内核空间，3GB属于用户空间（进程虚拟内存空间）。Linux2.6的默认线程栈大小是8M（通过ulimit -a查看），对于多线程，在创建线程的时候系统会为每一个线程预分配线程栈地址空间，也就是8M的虚拟内存空间。线程数量太多时，线程栈累计的大小将超过进程虚拟内存空间大小（计算时需要排除程序文本、数据、共享库等占用的空间），这就是实验中出现的“段错误”的原因。
	* Linux2.6的默认线程栈大小可以通过 ulimit -s 命令查看或修改，我们可以计算出线程数的最大上线: (1024*1024*1024*3) / (1024*1024*8) = 384，实际数字应该略小与384，因为还要计算程序文本、数据、共享库等占用的空间。在当今的稍显繁忙的WEB服务器上，突破384的并发访问并不是稀 罕的事情，要继续下面的实验需要将默认线程栈的大小减小，但这样做有一定的风险，比如线程中的函数分配了大量的自动变量或者函数涉及很深的栈帧（典型的是 递归调用），线程栈就可能不够用了。可以配合使用POSIX.1规定的两个线程属性guardsize和stackaddr来解决线程栈溢出问 题，guardsize控制着线程栈末尾之后的一篇内存区域，一旦线程栈在使用中溢出并到达了这片内存，程序可以捕获系统内核发出的告警信号，然后使用 malloc获取另外的内存，并通过stackaddr改变线程栈的位置，以获得额外的栈空间，这个动态扩展栈空间办法需要手工编程，而且非常麻烦。
* [页面置换算法](https://blog.csdn.net/yinyang_ddl/article/details/89439471)
	* 全局页面置换算法
		* 工作集和驻留集（充分权衡多进程的并发性和单个进程的缺页率） 
	* 局部页面置换算法 
		* 进程内的页面
* [Telnet用法](https://blog.csdn.net/u011889811/article/details/43356075?utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7EBlogCommendFromBaidu%7Edefault-3.nonecase&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7EBlogCommendFromBaidu%7Edefault-3.nonecase)
* [task_struct结构体成员详解](https://blog.csdn.net/bit_clearoff/article/details/54292300)
	* csapp的p581只对mm做了阐述
	* 具体包括：进程标识符、进程状态、进程标记符、表示进程亲属关系的成员、ptrace系统调用、进程调度优先级、进程地址空间	 
* [并查集](https://zhuanlan.zhihu.com/p/93647900/)
	* 执行路径压缩会改变子树的高度(rank)，因此要想保证rank的准确性，就不要把路径压缩和按秩合并一起使用。或者是改rank的定义为子树的结点数量（而不是树高），原理是节点多的树倾向更高。
	* 无论将秩定义成子树高上界，还是子树节点数，按秩合并都是尝试合出最矮的树，并不保证一定最矮。