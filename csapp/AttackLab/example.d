
example.o：     文件格式 elf64-x86-64


Disassembly of section .text:

0000000000000000 <.text>:
   0:	bf fa 97 b9 59       	mov    $0x59b997fa,%edi
   5:	48 c7 c0 ec 17 40 00 	mov    $0x4017ec,%rax
   c:	ff e0                	jmpq   *%rax
   e:	48 c7 c7 70 dc 61 55 	mov    $0x5561dc70,%rdi
  15:	48 b9 35 39 62 39 39 	movabs $0x6166373939623935,%rcx
  1c:	37 66 61 
  1f:	48 89 0f             	mov    %rcx,(%rdi)
  22:	48 c7 c0 fa 18 40 00 	mov    $0x4018fa,%rax
  29:	ff e0                	jmpq   *%rax
