#  CS144
计算机网络课程笔记
## lab0 
* 实现了一个bytestream类，用于在有限的存储空间中存储无限的数据流。当写数据时，数据从后边扩展，读数据时，数据从前边pop。具体的实现见byte_stream.cc和byte_stream.hh。

## lab1
* 实现了一个stream_reassembler类，将乱序到达或者有重叠的字符串重新组装成连续的stream。核心是lab1文档中3.1节的那个图,如下。
![3.1](./image/StreamReassembler.png)
 * 用storage来储存乱序到达的数据，然后当storage有连续数据时立马加入到lab0中实现的bytestream中。把不按序到达的数据尽可能存放起来，一旦出现连续数据就进行交付，所谓交付就是指把连续数据从storage write到output（是一个bytestream）。然后storage就空出空间来能够继续接收不按序和按序到达的数据
 * 上图中绿色的已经被交付给了bytestream
* 本实验的核心代码是stream_reassembler.cc和stream_reassembler.hh，主要实现push_substring函数
* 实现过程的挑战：一开始没有彻底理解这个数据结构的作用，用的是优先队列的方法，比较繁琐；后来看了测试数据后，大概理解了stream_reassember的作用，就开始实现现在所看到的数据结构；对于边界条件和EOF比较麻烦，对于EOF的处理，加了一个新的private变量EOFindex标记EOF的位置就使得程序变得简单
## lab2
* 实验文档摘录
 * 需要实现接收TCP报文段，重装数据流，为了确认和流量控制需要从receiver返回给sender的数据。
 * 逻辑开始SYN和逻辑终止FIN占据单独的序列号（sequence number）。SYN的第一个序列号是ISN，第一个真实数据是ISN+1而不是ISN+0.
 * 需要注意区分序列号（sequence number）、绝对序列号（absolute sequence number）、数据流索引（stream index）这三个概念的区别
 * 本实验重点关注如下
![TCPlab2](./image/TCPlab2.png)
 * window size是first_unassembled和first_unacceptable之间的距离
* 实验过程
	* 首先是将uint64_t的绝对序列号转化成uint32_t的序列号，转换方法lab2文档中有。接着是将uint32_t的序列号根据checkpoint的基准转化成uint64_t的绝对序列号。
	* TCPsegment类中实现了header和payload数据的解析，也通过计算机网络自顶向下中对UDP数据可靠性检验的校验和描述的原理进行了检验。也可见[链接](https://en.wikipedia.org/wiki/IPv4_header_checksum)中对检验和(checksum)的描述。
	* 在看别人写的代码的过程中，要倾向于用广度而不是深度，初次看时重点在代码框架，每个函数或者类了解其输入输出即可。一定不要陷入某个函数的具体实现细节中出不来，在初次看时唯一需要深入细节的时候就是当函数的输入输出不明白，需要通过实现细节来了解输入输出。等熟悉了整个代码后，就可以对一些细节进行深入研究。
