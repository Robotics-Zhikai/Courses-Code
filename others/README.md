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
* [C++派生类指针和基类指针相互的转化和赋值](http://c.biancheng.net/view/260.html)
	* 配合深度探索C++对象模型深刻理解
* ROS操作系统
	* 设计的初衷，就是使机器人开发能够像计算机开发一样，屏蔽其底层硬件及接口的不一致，最终使得软件可以复用。ROS能够以统一消息格式来使得大家只需要关注算法层面的设计，而底层硬件的根本目的是接收各种各样的消息，如图像、数据等，各个硬件厂商将接收到的数据都统一到ROS所规定的统一消息格式下，即可让用户方便的使用各种开源的机器人相关算法。
	* ROS已经彻底重构，推出了实时性更强的2.0版本，解决了ROS实时性问题，未来很有可能被直接用于实际产品的研发，为推进工业级机器人和服务机器人的应用做出重要贡献
* ROS1与ROS2
	* [ROS与ROS2比较](https://blog.csdn.net/tuziaaa/article/details/103358145?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522162609279016780262576720%2522%252C%2522scm%2522%253A%252220140713.130102334..%2522%257D&request_id=162609279016780262576720&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~baidu_landing_v2~default-1-103358145.first_rank_v2_pc_rank_v29_1&utm_term=ROS2%E4%B8%8EROS1%E7%9A%84%E5%8C%BA%E5%88%AB&spm=1018.2226.3001.4187)
	* [ROS学习笔记-ROS1和ROS2](https://blog.csdn.net/weixin_51244852/article/details/116421194?ops_request_misc=&request_id=&biz_id=102&utm_term=ROS2%E4%B8%8EROS1%E7%9A%84%E5%8C%BA%E5%88%AB&utm_medium=distribute.pc_search_result.none-task-blog-2~all~sobaiduweb~default-5-.first_rank_v2_pc_rank_v29_1&spm=1018.2226.3001.4187)
	* [ros1 和 ros2的区别](https://mengleil.blog.csdn.net/article/details/104751581?utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7EBlogCommendFromBaidu%7Edefault-5.essearch_pc&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7EBlogCommendFromBaidu%7Edefault-5.essearch_pc)
	* ROS2的目标：支持多机器人系统、铲除原型与产品之间的鸿沟、支持微控制器、支持实时控制、跨系统平台支持
	* ROS2可靠性更高，不会出现一个master挂掉整个都挂掉的情形，是一个分布式的实现。且在网络可靠性较低时能够很好地处理。
* struct的内存占用
	```
	struct A {
		int a; //4
		bool b; //1
		bool * c; //8
		bool d; //1 注意bool变量是1字节
	};
	//这个占用内存空间24字节（注意不能是20字节，20字节的话到A[1].c就没法对齐了）
	struct A {
		int a; //4
		bool b; //1
		bool * c; //8
	};
	//这个占用内存空间16字节 
	//总之是按照某一数据类型存放的地址是该数据类型内存大小的倍数的原则来进行的
	//同时也要考虑到A的结构体数组在连续存放时应能够满足对齐要求
	```
* [sizeof() 类大小，空类大小](https://blog.csdn.net/liu_qiqi/article/details/9344627)
	* 空类实例化的对象的大小为1字节，为了能够使得每个对象都有特异的地址。
	* 1、为类的非静态成员数据的类型大小之和。2、由编译器额外加入的成员变量的大小，用来支持语言的某些特性（如：指向虚函数的指针）。3、为了优化存取效率，进行的边缘调整（对齐）。4、与类中的构造函数，析构函数以及其他的成员函数无关。
* C的结构体和C++结构体的区别
	* C的结构体内不允许有函数存在，C++允许有内部成员函数，且允许该函数是虚函数。所以C的结构体是没有构造函数、析构函数、和this指针的。
	* C的结构体对内部成员变量的访问权限只能是public，而C++允许public,protected,private三种。
	* C语言的结构体是不可以继承的，C++的结构体是可以从其他的结构体或者类继承过来的。
   	* 以上都是表面的区别，实际区别就是面向过程和面向对象编程思路的区别：C的结构体只是把数据变量给包裹起来了，并不涉及算法。而C++是把数据变量及对这些数据变量的相关算法给封装起来，并且给对这些数据和类不同的访问权限。C语言中是没有类的概念的，但是C语言可以通过结构体内创建函数指针实现面向对象思想。
* C++的结构体和C++类的区别
	* C++结构体内部成员变量及成员函数默认的访问级别是public,而c++类的内部成员变量及成员函数的默认访问级别是private。
	* C++结构体的继承默认是public，而c++类的继承默认是private。
* [在 C＋＋ 里是怎么实现 delete[] 的？](https://www.zhihu.com/question/291750903/answer/477557020)
* [map/unordered_map原理和使用整理](https://blog.csdn.net/Blues1021/article/details/45054159?utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7EBlogCommendFromMachineLearnPai2%7Edefault-2.essearch_pc&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7EBlogCommendFromMachineLearnPai2%7Edefault-2.essearch_pc)
	* map的内部实现是二叉平衡树(红黑树)；hash_map内部是一个hash_table一般是由一个大vector，vector元素节点可挂接链表来解决冲突，来实现.
	* 非频繁的查询用map比较稳定；频繁的查询用hash_map效率会高一些，c++11中的unordered_map查询效率会更高一些但是内存占用比hash_map稍微大点。unordered_map 就是 boost 里面的 hash_map 实现。其他性能特点见链接
* [C++---静态多态与动态多态](https://blog.csdn.net/qq_37934101/article/details/81365449)
* 红黑树
	* 红黑树特点
		* 根节点和NULL是黑节点
		* 从NULL到根节点的黑节点数量相同
		* 红节点的父节点和儿子节点都为黑节点 
	* [教你初步了解红黑树](https://blog.csdn.net/v_JULY_v/article/details/6105630) 
	* [红黑树，超强动静图详解，简单易懂](https://zhuanlan.zhihu.com/p/79980618)
	* [红黑树和AVL树（平衡二叉树）区别](https://blog.csdn.net/u010899985/article/details/80981053)
	* [为什么工程中都用红黑树，而不是其他平衡二叉树？](https://www.zhihu.com/question/27542473)
	* 红黑树能够以 O(logn) 的时间复杂度进行搜索、插入、删除操作。此外，由于它的设计，任何不平衡都会在三次旋转之内解决。
* [C与C++之间的相互调用](https://blog.csdn.net/this_is_me_anyway/article/details/79397018?utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7EBlogCommendFromMachineLearnPai2%7Edefault-3.readhide&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7EBlogCommendFromMachineLearnPai2%7Edefault-3.readhide)
	* 正因为他们两者编译函数的时候，生成的符号规则不一样，所以，在混合编程中，如果我们不进行任何处理，而相互调用的话，必然会出现在链接的时候，找不到符号链接的情况。
	* 解决方法是声明extern C