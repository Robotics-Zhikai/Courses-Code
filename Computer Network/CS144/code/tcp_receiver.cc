#include "tcp_receiver.hh"

// Dummy implementation of a TCP receiver

// For Lab 2, please replace with a real implementation that passes the
// automated checks run by `make check_lab2`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

void TCPReceiver::segment_received(const TCPSegment &seg) { //每当一个新的TCP报文段到达，则会调用一次这个segment_received
    if (seg.header().syn){
        _ISN = seg.header().seqno;
        _recievedSYN = 1; //标记已经收到过SYN了
    }
    //FIN和SYN可能同时置位
    if(_recievedSYN){ //只有收到过SYN才会进入到收数据的阶段
        if (seg.header().syn){ //SYN的握手信号也可能携带数据
            _reassembler.push_substring(seg.payload().copy(),0,seg.header().fin);
        }
        else{
            _reassembler.push_substring(seg.payload().copy(),
                unwrap(seg.header().seqno,_ISN,_recentAbsoluteSeqnum)-1 ,seg.header().fin); 
                //这个是push到数据流中的数据，所以unwrap后必须减一
                //就是absolute_sqeno和stream_index的区别
        }
        _recentAbsoluteSeqnum = _reassembler.readLastreassembledByte(); 
        //checkpoint为什么这样取在lab2文档中有表述
        //设置为这个之后，在stream index域中之后解析的TCPseqno就分布在[_recentAbsoluteSeqnum-2^31,_recentAbsoluteSeqnum+2^31]中
    }
    
}

optional<WrappingInt32> TCPReceiver::ackno() const 
{
    if(_recievedSYN==0)
        return {};
    else{
        if(_reassembler.stream_out().input_ended()) //这个需要注意！！
            return wrap(_reassembler.read_firstUnassembled()+2,_ISN); //加2是因为FIN所代表的TCPseg单独占据一个序号
        else
            return wrap(_reassembler.read_firstUnassembled()+1,_ISN); 
            //+1是因为要从streamindex转化到absolute seqno
    }
}

size_t TCPReceiver::window_size() const 
{
    return _reassembler.stream_out().remaining_capacity();
}
