#include "stream_reassembler.hh"

// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity) : _output(capacity),
                         _capacity(capacity),_storage(capacity,'\0'),_storageEmpty(capacity,1) {}

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order. index的初始值必定是0
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
    //lab1的pdf中的那个图很重要
    //整个的思路就是从那个图入手，把不按序到达的数据尽可能存放起来，一旦出现连续数据就进行交付，所谓交付就是指把连续数据从storage write到output
    //然后storage就空出空间来能够继续接收不按序和按序到达的数据
    size_t oldunread = _firstUnread;
    _firstUnread = _output.bytes_read(); //有可能在调用push_substring之前output调用了read，如果不加这一步的话，就没法收数据 
    //firstunread只在这里更新
    //这一步是读到在整个数据流中第一个unread的数据索引
    for(size_t i = 1;i<=_firstUnread-oldunread;i++){ //每次read时都要移动storage对应的width 然后需要把移动后的数据标记为空
        // _storage[(_capacity+_firstUnread-i)%_capacity] = '\0'; //不能把'\0'这个作为特殊字符，因为很有可能数据流中也有这样的字符
        _storageEmpty[(_capacity+_firstUnread-i)%_capacity] = true; // 标记为空 之所以加_capacity是为了防止负数的出现
    }
    
    if (eof){
        _EOFindex = index+data.size(); //得到EOF在全局数据流中的位置，这个EOF的数据可能由于存储空间不够被丢弃，本次没有被处理但是总会被处理
    }
    if ((_output.bytes_written()==_EOFindex && _EOFindex!=0) || (_EOFindex==0 && eof))
            _output.end_input(); //这个是为了防止一开始就空字符串加EOF 或者运行在中间空字符串+eof

    string newdata = data;
    size_t newindex = index;
    if(index+data.size()<=_firstUnassembled) //说明已经write到output里了，直接忽略就行
        return;
    else if (index>=_firstUnread && index-_firstUnread>=_capacity) //说明超出了storage的存储空间，直接忽略
        return;
    else if (_firstUnassembled-_firstUnread>=_capacity) //说明现在所有的存储空间都是unread的连续数据流内存，直接忽略
        return;
    else {
        //newindex = max(_firstUnread,index); 
        //不能是_firstUnread 因为如果是unread的话，后边的if(newindex==_firstUnassembled)步就进不去，不能保证连续数据立即交付
        newindex = max(_firstUnassembled,index); //这个的话是在_firstUnassembled处截断，肯定能保证立即交付
        newdata = string(data.begin()+(newindex-index),
                            data.begin()+(min(index+data.size(),_firstUnread+_capacity)-index));
    }
    //上边的代码是当出现超出存储空间之外的数据时对数据进行丢弃

    //在本函数前边的步骤就已经保证newdata不会超出存储空间造成storage数据被覆盖进而造成数据丢失
    for (size_t i=0;i<newdata.size();i++){ //把字符串装到storage中 并把Empty标记位置false，表示该为非空
        size_t Capindex = (newindex+i)%_capacity;
        // if (_storage[Capindex]=='\0')
        if(_storageEmpty[Capindex]==true)
            _unassembledbytes++; //只起到一个统计未装配字节数量的作用
        _storage[Capindex]=newdata[i];
        _storageEmpty[Capindex] = false;
    }
    _lastReassembledByte = newindex+newdata.size();

    if(newindex==_firstUnassembled){ //一旦找到了从_firstUnassembled开始的连续字节流，就写入output
        size_t i = newindex;
        string writtendata("");

        while(i!=_firstUnread+_capacity && (_storageEmpty[i%_capacity]==false)){ //第一个条件表示最多把所有capacity看一遍
                                                                                //第二个表示一旦遇到空的就停止，成为新的不连续段首字符
            writtendata.push_back(_storage[(i++)%_capacity]);
            _firstUnassembled++;
            _unassembledbytes--;
        }
        _output.write(writtendata);
        if ((_output.bytes_written()==_EOFindex && _EOFindex!=0) || (_EOFindex==0 && eof))//当把EOFindex标记的EOF字节写进去后，就可以endinput了
            _output.end_input(); //注意这个endinput的使用
    }
    

}

size_t StreamReassembler::unassembled_bytes() const { return _unassembledbytes; }

bool StreamReassembler::empty() const { 
    if (unassembled_bytes()==0)
        return true;
    else
        return false;
}
