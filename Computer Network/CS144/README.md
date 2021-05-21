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
*  