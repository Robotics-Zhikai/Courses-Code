#ifndef SPONGE_LIBSPONGE_TCP_SENDER_HH
#define SPONGE_LIBSPONGE_TCP_SENDER_HH

#include "byte_stream.hh"
#include "tcp_config.hh"
#include "tcp_segment.hh"
#include "wrapping_integers.hh"

#include <functional>
#include <queue>
#include <iostream>
#include <set>
using namespace std;

//! \brief The "sender" part of a TCP implementation.
//bytestream------>>>>TCP segment
//! Accepts a ByteStream, divides it up into segments and sends the
//! segments, keeps track of which segments are still in-flight,
//! maintains the Retransmission Timer, and retransmits in-flight
//! segments if the retransmission timer expires.
class TCPSender {
  private:
    //! our initial sequence number, the number for our SYN.
    WrappingInt32 _isn;

    //! outbound queue of segments that the TCPSender wants sent
    std::queue<TCPSegment> _segments_out{}; 
    //要想实际发送TCPsegment，直接push到这个里边，然后在外部的_segments_out就能实际发送TCPsegment，TCPSender类并不需要关系实际是怎么发送的
    //lab3文档里有讲

    //! retransmission timer for the connection
    unsigned int _initial_retransmission_timeout; //重传超时时间的初始值

    //! outgoing stream of bytes that have not yet been sent
    ByteStream _stream;

    //! the (absolute) sequence number for the next byte to be sent
    uint64_t _next_seqno{0};

    //由文档lab3的FAQ，为了降低实现的复杂度，突出主要矛盾，默认每次发送的序号范围没有重叠
    void FirstPushTO_segments_out(const TCPSegment & tcpseg){ //tcpseg第一次push到_segments_out中，若涉及到重传就不能用这个push
        size_t seqno64 = unwrap(tcpseg.header().seqno,_isn,_next_seqno);
        
        if (_segmentsStorageSEQset.find(seqno64)==_segmentsStorageSEQset.end() && seqno64>=_ackNum
                                             && seqno64+tcpseg.length_in_sequence_space()>next_seqno_absolute()){
          //_segmentsStorage中的seqno必须是不重复的，如果是重复的要发，就不应该是FirstPushTO_segments_out函数的功能，
          //而应该是RetransPushTO_segments_out的功能
          //且对于已经经过确认的肯定receiver已经接收了的数据不应该继续发送
          //且对于seqno64+tcpseg.length_in_sequence_space()<=next_seqno_absolute()的不能发送，那样的话会重复的

            cout<<"push seqno64 "<<seqno64<<" "<<seqno64+tcpseg.length_in_sequence_space() <<endl;
            _segmentsStorageSEQset.insert(seqno64); //为了确保没有重复的序号
            _segments_out.push(tcpseg);
            _next_seqno+=tcpseg.length_in_sequence_space();
            _BytesFlightNum+=tcpseg.length_in_sequence_space();
            // std::cout<<"_BytesFlightNum add "<<tcpseg.length_in_sequence_space()<<std::endl;
            _segmentsStorage.push(tcpseg); //暂时保存，等待可能的重发或者确认
            // cout<<"_windowSize1 "<<_windowSize<<endl;
            if (_windowSize!=0) //为了防止溢出到65535
                _windowSize-=tcpseg.length_in_sequence_space(); //每次发送时要更新sender认为的_windowSize
            // cout<<"_windowSize2 "<<_windowSize<<endl;
            if(!_clock.has_value()){ //lab3文档张3.1的第4点，每次发送时如果没有启动定时器，则启动定时器
                _clock = 0;
            }
        }
    }
    void RetransPushTO_segments_out(const TCPSegment& tcpseg){ //重传tcpseg
        _segments_out.push(tcpseg);
        if(!_clock.has_value()){ //lab3文档张3.1的第4点，每次发送时如果没有启动定时器，则启动定时器
          //不管是重传还是第一次传，只要传送，又判断clock不运行，就开始运行
            _clock = 0;
        }
    }

    std::optional<size_t> _clock = {}; //TCPsender的Clock,为{}时说明定时器没有开启
    unsigned int _RTO; //当前重传超时时间，超过这个变量就需要重传了
    unsigned int _consecu_retransTimes = 0; //连续重传计数
    size_t _ackNum = 0; //在ackNum(不含ackNum)之前的所有序号都被receiver确认过了

    std::queue<TCPSegment> _segmentsStorage{}; //储存所有发送过但未确认的TCPSegment
    set<size_t> _segmentsStorageSEQset{}; //储存在_segmentsStorage中的segment的序号，保证_segmentsStorage中序号没有重复

    size_t _BytesFlightNum = 0; //发送出去还没有被确认的字节数量，包括SYN和FIN
    uint16_t _windowSize = 0; 
    //综合考虑最近得知的receiver端的windowsize和sender发送出去的TCPseg并假设receiver全部收到，二者相减就是_windowSize（见ack_received的实现）
    //表示sender在不接受receiver端更新的windowsize的情况下，fillwindow最大能发送的新（从未被发送过）的字节数量
    optional<uint16_t> _INITrecv_window_size{}; //仅在每次ackreceive更新
    bool _windowSizeUpdate = 1; //功能见代码
    bool _NeedFillwindow = 1; //标志需要fillwindow

  public:
    //! Initialize a TCPSender
    TCPSender(const size_t capacity = TCPConfig::DEFAULT_CAPACITY,
              const uint16_t retx_timeout = TCPConfig::TIMEOUT_DFLT,
              const std::optional<WrappingInt32> fixed_isn = {});

    //! \name "Input" interface for the writer
    //!@{
    ByteStream &stream_in() { return _stream; }
    const ByteStream &stream_in() const { return _stream; }
    //!@}

    //! \name Methods that can cause the TCPSender to send a segment
    //!@{

    //! \brief A new acknowledgment was received
    void ack_received(const WrappingInt32 ackno, const uint16_t window_size);

    //! \brief Generate an empty-payload segment (useful for creating empty ACK segments)
    void send_empty_segment();

    //! \brief create and send segments to fill as much of the window as possible
    void fill_window();

    //! \brief Notifies the TCPSender of the passage of time
    void tick(const size_t ms_since_last_tick);
    //!@}

    //! \name Accessors
    //!@{

    //! \brief How many sequence numbers are occupied by segments sent but not yet acknowledged?
    //! \note count is in "sequence space," i.e. SYN and FIN each count for one byte
    //! (see TCPSegment::length_in_sequence_space())
    size_t bytes_in_flight() const;

    //! \brief Number of consecutive retransmissions that have occurred in a row
    unsigned int consecutive_retransmissions() const;

    //! \brief TCPSegments that the TCPSender has enqueued for transmission.
    //! \note These must be dequeued and sent by the TCPConnection,
    //! which will need to fill in the fields that are set by the TCPReceiver
    //! (ackno and window size) before sending.
    std::queue<TCPSegment> &segments_out() { return _segments_out; }
    //!@}

    //! \name What is the next sequence number? (used for testing)
    //!@{

    //! \brief absolute seqno for the next byte to be sent
    uint64_t next_seqno_absolute() const { return _next_seqno; }

    //! \brief relative seqno for the next byte to be sent
    WrappingInt32 next_seqno() const { return wrap(_next_seqno, _isn); }
    //!@}
};

#endif  // SPONGE_LIBSPONGE_TCP_SENDER_HH
