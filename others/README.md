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
* [C与C++之间的相互调用](https://blog.csdn.net/shaosunrise/article/details/81176880?utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7EBlogCommendFromMachineLearnPai2%7Edefault-1.readhide&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7EBlogCommendFromMachineLearnPai2%7Edefault-1.readhide)
	* [C++是如何调用C接口的？](https://zhuanlan.zhihu.com/p/80214770)
	* 正因为他们两者编译函数的时候，生成的符号规则不一样，所以，在混合编程中，如果我们不进行任何处理，而相互调用的话，必然会出现在链接的时候，找不到符号链接的情况。
	* 解决方法是声明extern C
	* 结合csapp第七章看
* C++编译
	* 单文件编译方法
		```
		touch main.cpp //新建一个main.cpp 并写对应的代码
		g++ main.cpp //编译main.cpp，会产生一个a.out的文件
		./a.out //可以直接运行

		g++ main.cpp -o hello //会产生一个名为hello的文件
		./hello //可以直接运行
		```
	* 工程编译方法 CMakeLists.txt
		```
		//CMakeLists.txt 文件的内容
		cmake_minimum_required( VERSION 2.8 )
		project( hello )
		add_executable( hello main.cpp ) //把main.cpp编译成一个可执行程序

		add_library( hello1 hello.cpp ) //外边有一个hello.cpp文件，生成一个库，会生成一个libhello1.a的文件。需要定义一个hello.h hello.cpp
		
		add_executable( useHello useHello.cpp ) //useHello.cpp用到了hello.h中的函数。这是经过了编译器
		target_link_libraries( useHello hello1 ) //将库与useHello进行链接。这是经过了链接器
		```
		```
		cmake . //在当前目录下生成makefile等一系列中间文件,但这样的话比较乱，把原来的不是自动生成的文件给掩盖了，因此可以考虑在build中进行编译的操作
		mkdir build
		cd build 
		cmake .. 
		make 	//实际进行编译的操作，输出一个hello的文件 
		./hello
		```	
* [gdb调试coredump](https://blog.csdn.net/qq_39759656/article/details/82858101?utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7EsearchFromBaidu%7Edefault-2.pc_relevant_baidujshouduan&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7EsearchFromBaidu%7Edefault-2.pc_relevant_baidujshouduan)
	* [coredump产生的几种可能情况](https://blog.csdn.net/ydyang1126/article/details/51769010)
	* 进程运行时在突然崩溃的那一刻的一个内存快照；操作系统在程序发生异常而异常在进程内部又没有被捕获的情况下，会把进程此刻内存、寄存器状态、运行堆栈等信息转储保存在一个文件里。coredump是一个二进制文件，可以使用gdb进行调试
	* 可以使用info frame查看栈帧信息
	* Segmentation fault(core dumped)，比如free一个没有经过malloc的内存或者引用一个非法指针会出现这个fault
	* 通过设置，可以使得程序coredump的时候在指定路径按照指定的规则命名生成core文件
	* gdb打开core文件的格式为：gdb CorefilePath corefilename
	* gdb打开core文件时，如果显示没有调试信息，是因为之前编译的时候没有带上-g选项，没有调试信息是正常的，实际上它也不影响调试core文件。可在gcc时加上-g调试选项.可以用where或者bt查看出现异常的原文件所在详细位置。
	* 往往正常发布环境是不会带上调试信息的，因为调试信息通常会占用比较大的存储空间，一般都会在编译的时候把-g选项去掉.没有调试信息的情况下可先gdb打开core，然后bt得到coredump堆栈，f num跳到自己写的最下层的函数中，然后输入disassemble打开该函数的反汇编代码，就可从标记的箭头处推测出coredump的位置及原因。
* [weak_ptr浅析](https://blog.csdn.net/albertsh/article/details/82286999)
	* 用来解决shared_ptr的引用计数机制造成的循环引用
* 自动驾驶基础架构
	* [pony.ai分享](https://zhuanlan.zhihu.com/p/92763922)
		* 自动驾驶车载系统（这里介绍的是各个模块，但最后把他们串联起来，靠的是我们的自动驾驶软件系统，这就是自动驾驶的车载系统。很多自动驾驶企业使用的是 ROS 的一套工业系统，而 Pony.ai 是从第一行代码开始，写了一套 PonyBrain，自研的多层次自动驾驶车载系统）
			* 多模块调度运行
			* 模块间的消息通信
			* 车载计算资源的分配与管理（内存/CPU/GPU等）
				* 这就需要我们对内存、CPU、GPU 等有足够的优化，做到定制化的车载计算资源分配与管理。
				* 合理的架构避免大数据拷贝等严重影响性能的逻辑。
				* 依据模块逻辑分配合适的计算资源，如内存、CPU、GPU 等。
				* 定期对整个系统 Profile 分析系统的性能瓶颈。
			* 日志记录
				* 我们所有的测试数据回来都需要一整套的 Pipeline 去做自动化的分析，然后帮我们评判出有意义的数据，给到测试工程师或者研发工程师，进行进一步的分析去使用，然后进一步提升我们的模型。
			* 监控与报警
		* 大规模仿真平台
			* 支持真实路测收集的场景，我们的路测数据非常的多，数据回来之后，通过 Data Pipeline 自动更新这些有意义和有意思的场景，我们会根据当时的场景改动相应的模块，然后会在仿真系统重跑当时的场景，来判断新的方法是否 work
			* 支持人工和随机生成的场景，这样的一些仿真的场景，也是非常的重要的
			* 我们将整个仿真系统并行部署在分布式计算平台中，这才可能满足我们单天 30W 公里以上的仿真测试，并且这个数据还在不断增长。
		* 数据基础架构
			* 如何存储海量的数据，如何支持快速的访问
				* 依据特定的使用场景设计合理的存储格式：以便于车载系统记录、大规模数据分析（数据回来之后，需要有方法进行分析，找出有意义的数据）、部分数据访问、文件系统存储（如何高效的利用文件系统）等
				* 选择合适的存储系统
			* 如何进行数据处理
			* 如何进行数据同步，如何把不同区域、路测数据、车载数据同步到数据集中，如何让不同办公区的工程师都可以使用这些数据，对数据同步来说是一个很大的挑战
		* 车队运营基础平台
		* 可视化平台与人机接口
	* [滴滴](https://blog.csdn.net/DiDi_Tech/article/details/111714049)
		* 操作系统
			* 在操作系统的选择方面，是选择更加熟悉、资源更加丰富的 Linux 系统，还是选择实时性更有保障、更加安全可靠的 RTOS 系统
		* 中间件
		* 传感器驱动
		* 监控系统、日志、人机交互界面
		* 云
* I/O多路复用
	* [深入理解select、poll和epoll及区别,这个看看就行了说的不清楚](https://blog.csdn.net/wteruiycbqqvwt/article/details/90299610?utm_medium=distribute.pc_relevant_t0.none-task-blog-2%7Edefault%7EBlogCommendFromMachineLearnPai2%7Edefault-1.control&depth_1-utm_source=distribute.pc_relevant_t0.none-task-blog-2%7Edefault%7EBlogCommendFromMachineLearnPai2%7Edefault-1.control)
	* 这个系列说的非常清楚，底下的评论也非常有用
		* [网卡接收数据，如何知道接收到了数据](https://zhuanlan.zhihu.com/p/63179839)
			* Q:数据经由网卡传送到内存（步骤②），然后网卡通过中断信号通知cpu有数据到达，cpu执行中断程序（步骤③）。此处的中断程序主要有两项功能，先将网络数据写入到对应socket的接收缓冲区里面（步骤④），再唤醒进程A（步骤⑤），重新将进程A放入工作队列中。select低效的另一个原因在于程序不知道哪些socket收到数据，只能一个个遍历。进程被唤醒后，程序并不知道哪些socket收到数据，还需要遍历一次。这是不是有矛盾？数据传到内存时，中断程序必然知道哪个socket收到程序，并写入对应缓冲区。那为啥select时中断程序就不知道哪个socket收到程序？
			* A:内核当然知道哪个socket收到数据，不然select返回的时候也不能正确标识有数据的槽位。问题是select的实现机制造成的，他没有明确指出哪些描述符有数据，哪些没有，这就需要应用程序遍历一次才知道。当然这是历史原因，select这种实现当时应该满足需要了，但没料到网络发展这么迅猛，对性能要求越来越高。其实就是如果想收到数据就知道是哪个socket的话需要耗费空间进行记忆，但是为了不耗费空间，只能是以时间的消耗去遍历。
		* [内核接收网络数据全过程](https://zhuanlan.zhihu.com/p/64138532)
		* [epoll原理和流程](https://zhuanlan.zhihu.com/p/64746509)
			* select返回后还要来个for loop去检查所有fd;epoll返回的只有状态变了的fd.
			* O(1)是因为epoll的回调机制，在执行epoll_ctl时，除了把socket放到对应的红黑树上之外，还会给内核中断处理程序注册一个回调函数(中断系统里注册一个监听回调函数都是操作系统做的事情)，告诉内核，如果这个句柄的中断到了，就把它放到准备就绪rdlist链表里。所以，当一个socket上有数据到了，内核除了把网卡上的数据copy到内存中，还会把该socket插入到准备就绪链链表里。对于select中断处理程序仅是把数据从网卡拷贝到内存，而epoll多了一个回调函数的执行，即把有事件发生的文件描述符引用放到就绪链表中.
			* 而所有添加到epoll中的事件都会与设备(网卡)驱动程序建立回调关系，也就是说，当相应的事件发生时会调用这个回调方法。这个回调方法在内核中叫ep_poll_callback,它会将发生的事件添加到rdlist双链表中。
	* [Linux下I/O多路复用系统调用(select, poll, epoll)介绍](https://zhuanlan.zhihu.com/p/22834126)
		* 采用回调的方式，效率提升。只有活跃可用的fd才会调用callback函数，也就是说 epoll 只管你“活跃”的连接，而跟连接总数无关，因此在实际的网络环境中，epoll的效率就会远远高于select和poll。
	* select 
		* 时间复杂度O(n),无差别轮询所有流，找出能读出数据，或者写入数据的流，对他们进行操作
		* 每次调用select，都需要把fd集合从用户态拷贝到内核态，这个开销在fd很多时会很大
		* 每次调用select都需要在内核遍历传递进来的所有fd，这个开销在fd很多时也很大
	* poll 
		* 时间复杂度O(n),它将用户传入的数组拷贝到内核空间，然后查询每个fd对应的设备状态， 但是它没有最大连接数的限制，原因是它是基于链表来存储的.
		* 管理多个描述符也是进行轮询
		* poll和select同样存在一个缺点就是，包含大量文件描述符的数组被整体复制于用户态和内核的地址空间之间，而不论这些文件描述符是否就绪，它的开销随着文件描述符数量的增加而线性增大。
	* epoll
		* 时间复杂度O(1),epoll会把哪个流发生了怎样的I/O事件通知我们
		* 每次注册新的事件到epoll句柄中时（在epoll_ctl中指定EPOLL_CTL_ADD），会把所有的fd拷贝进内核，而不是在epoll_wait的时候重复拷贝。epoll保证了每个fd在整个过程中只会拷贝一次。
		* 内存拷贝，利用mmap()文件映射内存加速与内核空间的消息传递；即epoll使用mmap减少复制开销
		* LT模式下，只要这个fd还有数据可读，每次 epoll_wait都会返回它的事件，提醒用户程序去操作，而在ET（边缘触发）模式中，它只会提示一次，直到下次再有数据流入之前都不会再提示了，无论fd中是否还有数据可读
	* select，poll，epoll本质上都是同步I/O，因为他们都需要在读写事件就绪后应用层自己负责进行读写，也就是说这个读写过程是阻塞的，而异步I/O则无需自己负责进行读写，异步I/O的实现会负责把数据从内核拷贝到用户空间。 	
* 临界资源
	* 临界资源是一次仅允许一个进程使用的共享资源。各进程采取互斥的方式，实现共享的资源称作临界资源。属于临界资源的硬件有，打印机，磁带机等；软件有消息队列，变量，数组，缓冲区等。诸进程间采取互斥方式，实现对这种资源的共享。
* 临界区
	* 每个进程中访问临界资源的那段代码称为临界区（criticalsection），每次只允许一个进程进入临界区，进入后，不允许其他进程进入。不论是硬件临界资源还是软件临界资源，多个进程必须互斥的对它进行访问。多个进程涉及到同一个临界资源的的临界区称为相关临界区。使用临界区时，一般不允许其运行时间过长，只要运行在临界区的线程还没有离开，其他所有进入此临界区的线程都会被挂起而进入等待状态，并在一定程度上影响程序的运行性能。
* [缓存一致性协议（MESI协议）](https://blog.csdn.net/zxp_cpinfo/article/details/53523697?utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7EBlogCommendFromBaidu%7Edefault-5.control&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7EBlogCommendFromBaidu%7Edefault-5.control)
	* 通过在总线加LOCK#锁的方式
	* 通过缓存一致性协议
	* 这2种方式都是硬件层面上提供的方式。
* 物理核（core）、逻辑核（thread）、线程
	* 物理核：物理核数量=cpu数(机子上装的cpu的数量)*每个cpu的核心数
	* 逻辑核：所谓的4核8线程，4核指的是物理核心。通过超线程技术，用一个物理核模拟两个虚拟核，每个核两个线程，总数为8线程。在操作系统看来是8个核，但是实际上是4个物理核。
	* 通过超线程技术可以实现单个物理核实现线程级并行计算，但是比不上性能两个物理核。---超线程技术实现了单个物理核心同时两个线程，也就是别人常说的虚拟内核数。比如单物理核心实现的双线程，它同时可以处理两个线程，它的物理核心数其实是是1个，通过Hyperthreading技术实现的线程级并行(Thread Lever Parallelism)。
	* core i7 10 Gen处理器是Six-core/2 computing threads per core。
* 可以用加锁复制技术将线程不安全的函数变成线程安全的函数。
* [多线程死锁的产生以及如何避免死锁](https://blog.csdn.net/ls5718/article/details/51896159)
* [C++11中静态局部变量初始化的线程安全性](https://blog.csdn.net/imred/article/details/89069750)
	* 在C++11标准中，要求**局部静态变量初始化**具有线程安全性，所以我们可以很容易实现一个线程安全的单例类
* [C++ 单例模式总结与剖析](https://www.cnblogs.com/sunchaothu/p/10389842.html)
* [单例模式——饿汉模式](https://blog.csdn.net/lvyibin890/article/details/81943637?utm_source=app)
	* 饿汉模式：像一个饿汉一样，不管需不需要用到实例都要去创建实例，即在类产生的时候就创建好实例，这是一种空间换时间的做法
	* 懒汉模式：像一个懒汉一样，需要用到创建实例了程序再去创建实例，不需要创建实例程序就“懒得”去创建实例，这是一种时间换空间的做法，这体现了“懒汉的本性”
* [#define与const的区别](https://blog.csdn.net/qew2017/article/details/104857491)
	* #define不做类型检查，没有作用域限制