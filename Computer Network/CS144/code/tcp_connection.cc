#include "tcp_connection.hh"

#include <iostream>

// Dummy implementation of a TCP connection

// For Lab 4, please replace with a real implementation that passes the
// automated checks run by `make check`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

size_t TCPConnection::remaining_outbound_capacity() const 
{
    return _sender.stream_in().remaining_capacity();
}

size_t TCPConnection::bytes_in_flight() const 
{
    return _sender.bytes_in_flight();
}

size_t TCPConnection::unassembled_bytes() const 
{
    return _receiver.unassembled_bytes();
}

size_t TCPConnection::time_since_last_segment_received() const 
{
    return _clock-_segReceivedTIME; //size_t根本不可能溢出，从0加到溢出需要5亿多年
}

void TCPConnection::segment_received(const TCPSegment &seg) 
{
    if(seg.header().rst){
        _sender.stream_in().set_error();
        _receiver.stream_out().set_error();
        _isRST = 1;
    }
    else{
        
        _receiver.segment_received(seg);
        _segReceivedTIME = _clock;
        if (seg.header().syn==1)
            _synReceived = 1;
        if (_synReceived){ //只有接收到syn过才会给予反应，否则一开始给个ack都会给反应
            if (seg.header().ack || seg.header().syn ){ //有可能syn并没有置ack
                if (seg.header().ack){ //如果不加这个if，当作为listen的时候，第一个到达的SYN的ackno是不合法的，会导致_sender的_ackNum变乱
                    _sender.ack_received(seg.header().ackno,seg.header().win);
                    // cout <<"segment_received"<<endl;
                }
    
                // bool sedempt = unwrap(seg.header().seqno,_receiver.read_ISN(),_receiver.stream_out().bytes_written())+
                //                                         seg.length_in_sequence_space()
                //             <unwrap(_receiver.ackno().value(),_receiver.read_ISN(),_receiver.stream_out().bytes_written());
                // cout<<"bool "<<sedempt<<endl;
                // seg.header().seqno+seg.length_in_sequence_space() 
                if (/*( seg.header().syn||seg.header().fin) &&*/ _sender.stream_in().buffer_empty() && seg.length_in_sequence_space()!=0 && !_listen){ 
                    //接收第二次握手信号且_sender.stream_in()数据为空时给对方发送一个空的确认
                    //接收一个fin信号且数据为空时给对方发送一个空的确认
                    //实际上不应该按照case编程，把握住不应该给空的回ack，然后每个buffer_empty都应该回复一个空ack
                    _sender.send_empty_segment();
                }
                else{  //如果是_listen的话，需要在接收到syn后回一个syn，也就不能发一个空segment了
                    _sender.fill_window();
                    
                }
                flush_sender_segments_out();
                if (_outboundEndSent && _sender.bytes_in_flight()==0){
                    _outboundfullyack = true;
                }
            }
        }
        
        if (_receiver.stream_out().input_ended()){
            _inboundEnd = true;
        }
        if (_inboundEnd && !_outboundEndSent){
            _linger_after_streams_finish = false;
        }
    }
}

bool TCPConnection::active() const 
{
    if (_lingerdone)
        return 0;
    if (_isRST)
        return 0;
    if (_inboundEnd && _outboundEndSent && _outboundfullyack){
        if (_linger_after_streams_finish==false){
            return 0;
        }
        else{
            return 1;
        }
    }
    return 1;
}

size_t TCPConnection::write(const string &data) {
    size_t writeNum = _sender.stream_in().write(data);
    _sender.fill_window();
    flush_sender_segments_out();
    return writeNum;
}

//! \param[in] ms_since_last_tick number of milliseconds since the last call to this method
void TCPConnection::tick(const size_t ms_since_last_tick)  //lab4文档中 when time passes
{
    _clock+=ms_since_last_tick;
    _sender.tick(ms_since_last_tick);
    if (_sender.consecutive_retransmissions()>TCPConfig::MAX_RETX_ATTEMPTS){
        // cout<<"ssssssssssssssssssssss1"<<endl;
        pushto_segments_out_RST();
    }
    else{
        flush_sender_segments_out();//_sender tick可能会遇到超时重传，因此需要每次都及时flush
    }
    if (_inboundEnd && _outboundEndSent && _outboundfullyack && _linger_after_streams_finish){
        if (time_since_last_segment_received()>=10*_cfg.rt_timeout){
            _lingerdone = 1;
            // cout<<"ssssssssssssssssssssss"<<endl;
        }
    }

}

void TCPConnection::end_input_stream() 
{
    _sender.stream_in().end_input();
    _sender.fill_window(); //end stream后要及时输出
    flush_sender_segments_out();
}

void TCPConnection::connect() 
{
    _listen = 0;
    _sender.fill_window();
    flush_sender_segments_out();
}

TCPConnection::~TCPConnection() {
    try {
        if (active()) {
            cerr << "Warning: Unclean shutdown of TCPConnection\n";
            pushto_segments_out_RST();
            // _sender.send_empty_segment();
            // _sender.segments_out().back().header().fin = 1;
            // flush_sender_segments_out();
            
            // Your code here: need to send a RST segment to the peer
        }
    } catch (const exception &e) {
        std::cerr << "Exception destructing TCP FSM: " << e.what() << std::endl;
    }
}
