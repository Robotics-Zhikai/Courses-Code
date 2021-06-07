#include "tcp_sender.hh"

#include "tcp_config.hh"

#include <random>
#include <iostream>

// Dummy implementation of a TCP sender

// For Lab 3, please replace with a real implementation that passes the
// automated checks run by `make check_lab3`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

//! \param[in] capacity the capacity of the outgoing byte stream
//! \param[in] retx_timeout the initial amount of time to wait before retransmitting the oldest outstanding segment
//! \param[in] fixed_isn the Initial Sequence Number to use, if set (otherwise uses a random ISN)
TCPSender::TCPSender(const size_t capacity, const uint16_t retx_timeout, const std::optional<WrappingInt32> fixed_isn)
    : _isn(fixed_isn.value_or(WrappingInt32{random_device()()}))
    , _initial_retransmission_timeout{retx_timeout}
    , _stream(capacity)
    , _RTO(retx_timeout) {}

uint64_t TCPSender::bytes_in_flight() const { return _BytesFlightNum; }

void TCPSender::fill_window() //SYN和FIN也占据window的空间 
//主要功能是尽可能填满receiver处得知的windowsize
{
    if (_NeedFillwindow){
        uint16_t LeftwindowSize = (_windowSize==0)?1:_windowSize; //剩余的能发的窗口 
        //lab3文档里这么说的 主要是为了当receiver空出缓存时，receiver的返回应答能够给出有效的windowsize，计算机网络自顶向下有描述

        if (_windowSizeUpdate==1){ 
        //只有ack_received才会置_windowSizeUpdate为1；也就是说只有更新了一次_windowSize时才会把_windowsize的0当做1
            _windowSizeUpdate = 0;
        }
        else {
            if (_windowSize==0) //防止出现：调用一次fill_window后，_windowSize已经等于0了，然后再继续重复调用fill_window，会造成不断的发数据，就起不到receiver端窗口限制的作用
                return;
        }
        
        if (next_seqno_absolute()!=0 && LeftwindowSize!=0 && _stream.eof()){ //发送过字节，但是一进来就是eof
            TCPSegment tcpseg;
            tcpseg.header().fin = 1;
            tcpseg.header().seqno = wrap(_stream.bytes_read()+1,_isn); //之所以+1是因为需要把一开始的SYN算上

            FirstPushTO_segments_out(tcpseg);
            return;
        }
        //从未发送一个字节 不能用_stream.Bytesread()判断是否从未发送一个字节，因为有可能第一个发送的是SYN
        if (next_seqno_absolute()==0){ 
            if (LeftwindowSize <= 2){
                TCPSegment tcpseg;
                tcpseg.header().syn = 1;
                tcpseg.header().seqno = wrap(0,_isn);
                tcpseg.payload() = _stream.read(LeftwindowSize-1);

                LeftwindowSize-=tcpseg.length_in_sequence_space();
                if (LeftwindowSize!=0 && _stream.eof()){
                    tcpseg.header().fin = 1;
                }
                
                FirstPushTO_segments_out(tcpseg);
                return ;
            }
            else{
                TCPSegment tcpseg;
                tcpseg.header().syn = 1;
                tcpseg.header().seqno = wrap(0,_isn);
                tcpseg.payload() = _stream.read(min(static_cast<size_t>(LeftwindowSize-1)
                                                            ,TCPConfig::MAX_PAYLOAD_SIZE));
                LeftwindowSize-=tcpseg.length_in_sequence_space();
                if (LeftwindowSize!=0 && _stream.eof()){
                    tcpseg.header().fin = 1;
                    LeftwindowSize--;
                }
                
                FirstPushTO_segments_out(tcpseg);
            }
        }

        while(LeftwindowSize!=0 && !_stream.buffer_empty()){
            TCPSegment tcpseg;
            tcpseg.header().seqno = wrap(_stream.bytes_read()+1,_isn);
            tcpseg.payload() = _stream.read(min(static_cast<size_t>(LeftwindowSize)
                                                            ,TCPConfig::MAX_PAYLOAD_SIZE));
            LeftwindowSize-=tcpseg.length_in_sequence_space();
            if (LeftwindowSize!=0 && _stream.eof()){ //只有有剩余的窗口容量且_stream读到了eof，才会fin=1
                tcpseg.header().fin = 1;
                LeftwindowSize--;

                FirstPushTO_segments_out(tcpseg);
                return ;
            }
            FirstPushTO_segments_out(tcpseg);
        }
    }
}

//! \param ackno The remote receiver's ackno (acknowledgment number)
//! \param window_size The remote receiver's advertised window size
void TCPSender::ack_received(const WrappingInt32 ackno, const uint16_t window_size) 
//主要功能是知道receiver端的windowsize，并根据receiver端发过来的ackno去消除_segmentsStorage中暂存的可能需要重发的数据
{
    uint64_t unwrapAckno = unwrap(ackno,_isn,next_seqno_absolute());
    if(_ackNum<=unwrapAckno) //当_ackNum<=unwrapAckno时，意味着有更新的确认到达，需要修改_ackNum
        _ackNum = unwrapAckno;

    _INITrecv_window_size = window_size; //_INITrecv_window_size只在这一处修改
    if (window_size==0){ //当收到的receiver显示的windowsize为0的时候，为了避免不能及时更新windowsize，_NeedFillwindow = 1
        _NeedFillwindow = 1;
    }
    //fillwindow调用的都是firstpush，当unwrapAckno+window_size<=next_seqno_absolute()意味着receiver的可接受窗口范围内都firstpush过了
    //也就_NeedFillwindow = 0了，即fillwindow什么都不需要做
    else if (window_size!=0 && unwrapAckno+window_size<=next_seqno_absolute()){
        _NeedFillwindow = 0;
        _windowSize = 0;
    }
    else{
        _windowSize = unwrapAckno+window_size-next_seqno_absolute(); 
        //注意这里更新_windowSize的方式 next_seqno_absolute>=unwrapAckno
        _NeedFillwindow = 1;
    }
    // _windowSize = window_size; //应该 不能这么简单的更新
    _windowSizeUpdate = 1; //跟fill window配合使用

    bool flag = 0;
    // cout<<"_segmentsStorage.front().header().seqno "<<_segmentsStorage.front().header().seqno.raw_value()<<endl;
    // cout<<"front "<<unwrap(_segmentsStorage.front().header().seqno,_isn,next_seqno_absolute())+_segmentsStorage.front().length_in_sequence_space()<<endl;
    // cout<<"unwrapAckno "<<unwrapAckno<<endl;
    while (!_segmentsStorage.empty() && (unwrap(_segmentsStorage.front().header().seqno,_isn,next_seqno_absolute())+
                                        _segmentsStorage.front().length_in_sequence_space()<=unwrapAckno)){
        uint64_t headerSeqno64 = unwrap(_segmentsStorage.front().header().seqno,_isn,next_seqno_absolute());
        // cout<<"pop headerSeqno64 "<<headerSeqno64 <<" "<<headerSeqno64+_segmentsStorage.front().length_in_sequence_space() <<endl;
        _segmentsStorageSEQset.erase(_segmentsStorageSEQset.find(headerSeqno64)); //为了保证所有的firstpush没有重复
        _BytesFlightNum-=_segmentsStorage.front().length_in_sequence_space();
        _segmentsStorage.pop(); //弹出所有小于ackno的段
        flag = 1;
    }
    if (flag){ //如果receiver成功确认了新的数据，则进入这个if  定时器管理
        _RTO = _initial_retransmission_timeout;
        if (!_segmentsStorage.empty()) //如果还有任何未确认的数据，则_clock置0
            _clock = 0;
        _consecu_retransTimes = 0;
    }
}

//! \param[in] ms_since_last_tick the number of milliseconds since the last call to this method
void TCPSender::tick(const size_t ms_since_last_tick) //主要是对定时器进行管理，并进行重传
{
    if(_clock.has_value()){ //如果定时器开启，则把每次的ms_since_last_tick加进去
        _clock = ms_since_last_tick + _clock.value();
    }
    
    if (_segmentsStorage.empty()){ //如果所有发送的TCPseg都被确认，则停止定时器 lab文档3.1-5
        _clock = {};
    }
    if (_clock.has_value() && _clock>=_RTO){ //lab文档3.1-6
        RetransPushTO_segments_out(_segmentsStorage.front()); //重传最久没有获得确认的seg，队列数据结构保证了重传front即可
        if (!_INITrecv_window_size.has_value() || _INITrecv_window_size!=0){ 
        //当一开始从未有ackreceiveed时，SYN需要指数backoff去发
        //一旦有了一次ackreceiveed，_INITrecv_window_size有值，_INITrecv_window_size!=0时才需要指数backoff发
        //否则等于0的话不进行RTO的倍增，因为此时需要高效得不断地给receiver重发，期待能够得到一个新的windowsize
            _consecu_retransTimes++;
            _RTO*=2; //exponential backoff
        }
        _clock = 0;
    }
}

unsigned int TCPSender::consecutive_retransmissions() const { return _consecu_retransTimes; }

void TCPSender::send_empty_segment() 
{

}
