实现了基于生产者消费者模型的并发服务器；缓存是读者优先的读者写者模型，遵循LRU；
缓存的实现基于链表，实现了动态分配和删除替换小object的功能;
如csapp中文课本P678所示，可能会出现broken pipe，导致服务器进程退出，只需要加一个sigpipe_handler信号处理函数即可;
同时经过分析，write出现brokenpipe也不会造成任何内存泄漏;

用 valgrind --tool=memcheck --leak-check=full ./proxy 10802 命令检查了，发现没有内存泄露.
