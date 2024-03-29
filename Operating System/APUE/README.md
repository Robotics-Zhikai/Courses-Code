# APUE
## 第11章 线程
* 一个进程的所有信息对该进程的所有线程都是共享的，包括可执行程序的代码、程序的全局内存和堆内存、栈以及文件描述符
* pthread_join，调用线程将一直阻塞，直到指定的线程调用pthread_exit、从启动例程中返回或者被取消。
	* 当一个线程通过调用pthread_exit退出或者简单地从启动历程中返回时，进程中的其他线程可以通过调用pthread_join函数获得该线程的退出状态
	* pthread_create和pthread_exit函数的无参数指针参数可以传递的值不止一个，这个指针可以传递包含复杂信息的结构的地址。需注意的是结构所使用的的内存在调用者完成调用以后必须仍然是有效的（涉及到线程栈的销毁）
* 默认情况下，线程的终止状态会保存直到对该线程调用pthread_join。但如果线程已经被分离，那么线程的底层存储资源可以在线程终止时立即被收回。在线程被分离后，我们不能用pthread_join函数等待它的终止状态，因为对分离状态的线程调用pthread_join会产生未定义行为。可以调用pthread_detach分离线程。
* 线程同步
	* 互斥量（用pthread的互斥接口来保护数据，确保同一时间只有一个线程访问数据）
		* 互斥变量是用pthread_mutex_t数据类型表示的
		* 对互斥量加锁有一个函数，如果已上锁，调用线程将阻塞直到互斥量被解除
		* 如果线程不希望互斥量被加锁时调用lock被阻塞，那么可以用try lock函数
		* 常用的函数有
			```
			pthread_mutex_t //互斥量的数据类型

			pthraed_mutex_init //初始化互斥量
			pthread_mutex_destroy	//对于调用malloc动态分配的互斥量，在释放内存前需要调用此函数
			pthread_mutex_lock //对互斥量进行加锁
			pthread_mutex_trylock  //对互斥量尝试进行加锁，如果已经锁住，那么不阻塞，而是返回EBUSY
			pthread_mutex_unlock  //对互斥量解锁
			```
	* 避免死锁
		* 如果线程试图对同一个互斥量加锁两次，那么它自身就会陷入死锁状态
		* 两个线程都在相互请求另一个线程拥有的资源，那么这两个线程都无法向前运行，于是就产生死锁
		* 可能出现的死锁只会发生在一个线程视图锁住另一个线程以相反的顺序锁住的互斥量。也就是说以相同的顺序加锁可以避免死锁。 
	* 函数pthread_mutex_timedlock
		* 与pthread_mutex_lock是基本等价的，但是可以指定愿意等待的绝对时间。在超时时间到达后，不会对互斥量加锁，而是返回错误码ETIMEDOUT
	* 读写锁（允许更高的并行性，读写锁也叫共享互斥锁）
		* 只有在线程搜索作业的频率远远高于增加或删除作业时，使用读写锁才可能改善性能
		* 读写锁可以有三种状态：读模式下加锁状态，写模式下加锁状态，不加锁状态。一次只有一个线程可以占有写模式的读写锁，但是多个线程可以同时占有读模式的读写锁。
		* 当读写锁是写加锁状态时，在解锁之前，所有试图对这个锁加锁的线程都会被阻塞。当读写锁是读加锁状态时，所有试图以读模式对它进行加锁的线程都可以得到访问权，但是任何写线程都会阻塞。只要有一个写，那么之后的读就会被锁住，这样可以避免读模式锁长期占用。
		* 常用函数
			```
			pthread_rwlock_t //读写锁的数据类型

			pthread_rwlock_init //读写锁初始化
			pthread_rwlock_destroy //在释放读写锁内存前必须调用本函数销毁

			pthread_rwlock_rdlock //读模式下锁定读写锁
			pthread_rwlock_wrlock //写模式下锁定读写锁
			pthread_rwlock_unlock //不管以何种方式锁住读写锁，都可以调用此函数解锁
			
			pthread_rwlock_tryrdlock
			pthread_rwlock_trywrlock
			```
	* 带有超时的读写锁（跟互斥量的对应概念是类似的）
		* 使应用程序在获取读写锁时避免陷入永久阻塞状态,到指定的绝对时间还没有获得锁的话函数就返回ETIMEDOUT错误
		* 相关函数是
			```
			//一般来讲，函数成功调用返回0，否则返回非零的错误编号
			pthread_rwlock_timerdlock
			pthread_rwlock_timewrlock
			```
	* 条件变量
		* 条件变量是线程可用的另一种同步机制，与互斥量一起使用时允许线程以无竞争的方式等待特定条件发生
		* 条件变量是线程的另外一种同步机制，这些同步对象为线程提供了会合的场所，理解起来就是两个（或者多个）线程需要碰头（或者说进行交互-一个线程给另外的一个或者多个线程发送消息），我们指定在条件变量这个地方发生，一个线程用于修改这个变量使其满足其它线程继续往下执行的条件，其它线程则接收条件已经发生改变的信号。
		* 并发有两大需求，一是互斥，二是等待。互斥是因为线程间存在共享数据，等待则是因为线程间存在依赖。
		* 注意！！！线程进入休眠状态等待
		* 几个讲的比较好的链接
			* [深入解析条件变量](https://www.cnblogs.com/harlanc/p/8596211.html)
			* [怎样理解条件变量](https://www.zhihu.com/question/68017337/answer/796332672)
			* [条件变量实例](https://www.cnblogs.com/jacksu-tencent/p/3366238.html)
		* 主要函数
			```
			pthread_cond_t //条件变量的数据类型

			pthread_cond_init //条件变量是动态分配的，则需要使用此函数进行初始化
			pthread_cond_destroy //在释放条件变量的底层内存空间之前，调用此函数对条件变量进行反初始化
			
			pthread_cond_wait(cond,mutex) //等待条件变为真，同时需要有互斥量进行保护。具体的机制是：调用者把锁住的互斥量传给函数，函数然后原子地把线程放到等待条件的线程列表上，对互斥量解锁；该函数返回时互斥量再次被锁住。
			pthread_cond_timedwait //超时值指定我们愿意等待多长时间，如果超时到期时条件还是没有出现，则本函数将重新获取互斥量，然后返回ETIMEDOUT

			//有两个函数可以用于通知线程条件已经满足
			pthread_cond_signal //至少能唤醒一个等待该条件的线程
			pthread_cond_broadcast //唤醒等待该条件的所有线程
			```
		* 可以高效的实现生产者消费者模式，类似于事件触发
	* 自旋锁
		* 自旋锁与互斥量类似，但它不是通过休眠，而是通过忙等。可用于锁被持有的时间短，而且线程不希望在重新调度上花费太多成本的情况。
		* 主要函数
			```
			pthread_spinlock_t //自旋锁的数据结构

			pthread_spin_init //初始化，还能设置进程共享属性
			pthread_spin_destroy

			pthread_spin_lock
			pthread_spin_trylock
			pthread_spin_unlock
			```
		* 不要调用在持有自旋锁情况下可能会陷入休眠状态的函数。如果调用了这些函数，会由于其他线程忙等自旋锁而白白浪费CPU资源
	* 屏障
		* 使用户协调多个线程并行工作的同步机制。屏障允许每个线程等待，知道所有的合作线程都到达某一点，然后从该点继续执行。pthread_join就是一种屏障
		* 主要函数
			```
			pthread_barrier_t //屏障的数据结构
			
			pthread_barrier_init //对屏障进行初始化，初始化时可以使用count参数指定在允许所有线程继续运行之前，必须到达屏障的线程数目（屏障计数）
			pthread_barrier_destroy //进行反初始化

			pthread_barrier_wait(pthread_barrier_t*barrier)//调用本函数的线程在屏障计数未满足条件时，会进入休眠状态。如果该线程是最后一个调用本函数的，就满足了屏蔽计数，那么所有因为这个函数而休眠的线程都将被唤醒，同时这个函数返回
			```
		* 应用实例：多个子线程分割空间对大数据进行分别排序，最终由主线程merge
## 第12章 线程控制
* 同步属性
	* 互斥量属性
		* 允许相互独立的多个进程把同一个内存数据块映射到它们各自独立的地址空间中。就像多个线程访问共享数据一样，多个进程访问共享数据也需要同步。如果**进程共享**互斥量属性设置为PTHREAD_PROCESS_SHARED，那么从多个进程彼此之间共享的内存数据块中分配的互斥量就可以用于这些进程的同步。而当设置为PTHREAD_PROCESS_PRIVATE时，只有初始化的那个进程的多个线程可以使用。
	* 读写锁属性
		* 同样有进程共享的属性
	* 条件变量属性
		* 同样有进程共享的属性
	* 屏障属性
		* 同样有进程共享的属性
* 重入
	* 与信号处理程序类似，多个控制线程在相同的时间有可能调用相同的函数，如果一个函数在相同的时间点可以被多个线程安全的调用，就称该函数是线程安全的
* 线程和信号
	* 每个线程都有自己的信号屏蔽字，但是信号的处理是进程中所有线程共享的。也就是说每个线程用signal安装的信号处理函数对于所有线程来说是共享的。只有屏蔽字对于每个线程来说是特异的、自己的。每个信号都有一个pending位和block位，pending位置位时表示对应的信号来了，block置位时表示对应的信号屏蔽了，不起反应。
	* 可以用pthread_sigmask函数来设置线程自己的屏蔽字。
	* sigwait
		* 线程可以用sigwait函数等待一个或多个信号的出现。sigwait的输入参数有一个关注的信号集。如果该信号集的某个信号接收到了，那么sigwait将无阻塞返回，并在返回前置零该pending位。
		* 为了避免错误发生，线程在调用sigwait之前，必须屏蔽那些它在等待的信号。sigwait函数会原子地取消信号集的屏蔽状态，直到有新的信号进来，在返回之前，sigwait会恢复原来的屏蔽状态。
		* 使用sigwait的好处在于它可以简化信号处理，允许把异步产生的信号用同步的方式处理：为了防止信号中断线程，可以把信号加到每个线程的信号屏蔽字中，然后安排专用线程处理信号，这些专用线程可以进行函数调用，不需要担心在信号处理程序中调用哪些函数是安全的，因为这些函数调用来自正常的线程上下文，而非会中断线程正常执行的传统信号处理函数。**也就是说可以把所有的信号处理过程都由一个线程负责，其他的线程根本不需要考虑可能被信号打断的情况，前提是其他线程都屏蔽所有的信号**
		* 如果多个线程在sigwait中等待同一个信号，那么信号递送的时候，就只有一个信号能从sigwait中返回。如果一个信号被捕获将要去到signal安装的信号处理函数中，而且一个线程正在sigwait中等待同一信号，那么这时候由操作系统决定到底是去信号处理函数还是sigwait返回，不会出现两个同时发生的情况。实际上，要尽量避免对某一信号既signal安装信号处理函数，又sigwait该信号，见下边的链接。
	* [假想如下场景：多线程环境中，如果有一个信号，我们不想使用sigwait来处理，而使用进程默认的signal handler去处理，而其他信号都想被一个线程sigwait同步处理，那要如果实现呢？](https://blog.csdn.net/rikeyone/article/details/89203836)
		* 该信号想要触发signal handler，一定要能够被递送到进程，或者说一定要存在一个线程允许递送它，也就是不能完全阻塞。
		* 想要被sigwait接收的信号，一定是pending信号，那么一定要先阻塞对应的信号。
		* 为了防止工作线程系统调用被打断，创建一个线程处理信号，保证signal handler还是sigwait都在该特定线程运行。
	* 进程中的信号是递送到单个线程的。
		* pthread_kill可以发送信号到另一线程
		* 在多线程环境下，产生的信号是传递给整个进程的，一般而言，所有线程都有机会收到这个信号，进程在收到信号的的线程上下文执行信号处理函数，具体是哪个线程执行的难以获知。也就是说，信号会随机发个该进程的一个线程;也有可能是当一个信号与硬件故障相关，那么该信号一般会被发送到引起该事件的线程中去。
		* 发送信号给进程，哪个线程会收到？APUE说，在多线程的程序中，如果不做特殊的信号阻塞处理，当发送信号给进程时，由系统选择一个线程来处理这个信号。
		* 如果进程中，有的线程可以屏蔽了某个信号，而某些线程可以处理这个信号，则当我们发送这个信号给进程或者进程中不能处理这个信号的线程时，系统会将这个信号投递到进程号最小的那个可以处理这个信号的线程中去处理。[参考链接](http://blog.chinaunix.net/uid-12274566-id-3050955.html)
* 线程和I/O
	* pread和pwrite函数在多线程环境下非常有用，因为进程中的所有线程共享相同的文件描述符。用来解决并发线程对同一文件的读写问题。
## 第15章 进程间通信
* 管道（无名管道 pipe，配合fork close进行创建）
* FIFO （命名管道 需要跟path联系起来）
	* 一个给定的FIFO有多个写进程是常见的，如果不希望多个进程所写的数据交叉，必须考虑原子写操作
* XSI IPC （key转化到标识符，把IPC看成一种对象资源，有以下三类资源）
	* 消息队列
	* 信号量
	* 共享存储
		* 进程间共享内存，最快的IPC。可以用管道机制实现不同进程对共享内存的互斥访问。也可以用XSI IPC提供的信号量机制对共享内存进行互斥访问。
* POSIX信号量（意在解决XSI信号量接口的几个缺陷，有两种形式如下，差异在于创建和销毁的形式上）
	* 命名的
		* 命名信号量可以通过名字访问，因此可以被任何已知它们名字的进程中的线程使用（这个应该和XSI IPC中的信号量思路是差不多的）
	* 未命名的
		* 未命名信号量只存在于内存中，并要求能使用信号量的进程必须可以访问内存，这意味着它们只能应用在同一进程中的线程，或者不同进程中已经映射相同内存内容到他们的地址空间中的线程。
* [这里对进程间通信的总结很好](https://www.zhihu.com/question/23995948/answer/1802584436)
	* 管道、消息队列会涉及到用户态与内核态的数据拷贝，共享存储不涉及用户态与内核态的数据拷贝。
* [什么场景用共享内存，什么场景用匿名管道](https://www.zhihu.com/question/23995948/answer/136236554)
	* PIPE和FIFO用来实现进程间相互发送非常短小的、频率很高的消息；这两种方式通常适用于两个进程间的通信。
	* 共享内存用来实现进程间共享的、非常庞大的、读写操作频率很高的数据（配合信号量使用）；这种方式通常适用于多进程间通信。
	* 其他考虑用socket。这里的“其他情况”，其实是今天主要会碰到的情况：分布式开发。在多进程、多线程、多模块所构成的今天最常见的分布式系统开发中，socket是第一选择。