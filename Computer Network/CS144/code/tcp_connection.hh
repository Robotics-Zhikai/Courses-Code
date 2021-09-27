#ifndef SPONGE_LIBSPONGE_TCP_FACTORED_HH
#define SPONGE_LIBSPONGE_TCP_FACTORED_HH

#include "tcp_config.hh"
#include "tcp_receiver.hh"
#include "tcp_sender.hh"
#include "tcp_state.hh"

//! \brief A complete endpoint of a TCP connection
class TCPConnection {
  private:
    TCPConfig _cfg;
    TCPReceiver _receiver{_cfg.recv_capacity};
    TCPSender _sender{_cfg.send_capacity, _cfg.rt_timeout, _cfg.fixed_isn};

    void flush_sender_segments_out() 
    //将_sender中的_segments_out全部flush到TCPConnection的_segments_out
    {
        while(!_sender.segments_out().empty()){
            TCPSegment& tcpfront = _sender.segments_out().front();
            if (_receiver.ackno().has_value()){ 
              //比如一开始在connect建立连接时receiver啥都没收到，只发SYN，此时就不进这个if，因为ackno为空
                tcpfront.header().ack = 1;
                tcpfront.header().ackno = _receiver.ackno().value();
                tcpfront.header().win =  //如果窗口值大于uint16_t能表示的最大值，那么就做一个限幅
                (_receiver.window_size()>=numeric_limits<uint16_t>::max())?numeric_limits<uint16_t>::max():_receiver.window_size();
            }
            
            if (tcpfront.header().fin){
                _outboundEndSent = true;
            }
            cout<<"push"<<endl;
            _segments_out.push(tcpfront);
            // cout<<tcpfront.header().ack<<" "<<tcpfront.header().fin
            // <<(tcpfront.header()).ackno.raw_value()<<" "<<tcpfront.header().seqno.raw_value()<< endl;
            // cout<<"ssssssssssssssss"<<endl;
            _sender.segments_out().pop();
        }
    }
    void pushto_segments_out_RST()
    {
        TCPSegment seg;
        seg.header().rst = 1;
        seg.header().seqno = _sender.next_seqno();
        _isRST = 1;
        _sender.stream_in().set_error();
        _receiver.stream_out().set_error();
        // cout<<"_segments_out "<<_segments_out.size()<<endl;
        _segments_out.push(seg);
    }
    bool _isRST = 0; //标记是否发送RST或者接收RST
    bool _inboundEnd = 0; //标记inboundstream是否接收到了end
    bool _outboundEndSent = 0; //标记outbound stream是否发送过FIN，也就是sender是否发送过fin
    bool _outboundfullyack = 0; //标记outbound stream是否完全被ack
    bool _lingerdone = 0; //延迟是否结束
    size_t _clock = 0; //累积时钟存放在_clock中
    size_t _segReceivedTIME = 0; //存放最近的seg到达的时刻
    
    bool _listen = 1; //默认本端是listen的，调用connect后才知道是非listen
    bool _synReceived = 0; //默认SYN没有收到

    //! outbound queue of segments that the TCPConnection wants sent
    std::queue<TCPSegment> _segments_out{};

    //! Should the TCPConnection stay active (and keep ACKing)
    //! for 10 * _cfg.rt_timeout milliseconds after both streams have ended,
    //! in case the remote TCPConnection doesn't know we've received its whole stream?
    bool _linger_after_streams_finish{true};

  public:
    //! \name "Input" interface for the writer
    //!@{

    //! \brief Initiate a connection by sending a SYN segment
    void connect();

    //! \brief Write data to the outbound byte stream, and send it over TCP if possible
    //! \returns the number of bytes from `data` that were actually written.
    size_t write(const std::string &data);

    //! \returns the number of `bytes` that can be written right now.
    size_t remaining_outbound_capacity() const;

    //! \brief Shut down the outbound byte stream (still allows reading incoming data)
    void end_input_stream();
    //!@}

    //! \name "Output" interface for the reader
    //!@{

    //! \brief The inbound byte stream received from the peer
    ByteStream &inbound_stream() { return _receiver.stream_out(); }
    //!@}

    //! \name Accessors used for testing

    //!@{
    //! \brief number of bytes sent and not yet acknowledged, counting SYN/FIN each as one byte
    size_t bytes_in_flight() const;
    //! \brief number of bytes not yet reassembled
    size_t unassembled_bytes() const;
    //! \brief Number of milliseconds since the last segment was received
    size_t time_since_last_segment_received() const;
    //!< \brief summarize the state of the sender, receiver, and the connection
    TCPState state() const { return {_sender, _receiver, active(), _linger_after_streams_finish}; };
    //!@}

    //! \name Methods for the owner or operating system to call
    //!@{

    //! Called when a new segment has been received from the network
    void segment_received(const TCPSegment &seg);

    //! Called periodically when time elapses
    void tick(const size_t ms_since_last_tick);

    //! \brief TCPSegments that the TCPConnection has enqueued for transmission.
    //! \note The owner or operating system will dequeue these and
    //! put each one into the payload of a lower-layer datagram (usually Internet datagrams (IP),
    //! but could also be user datagrams (UDP) or any other kind).
    std::queue<TCPSegment> &segments_out() { return _segments_out; }

    //! \brief Is the connection still alive in any way?
    //! \returns `true` if either stream is still running or if the TCPConnection is lingering
    //! after both streams have finished (e.g. to ACK retransmissions from the peer)
    bool active() const;
    //!@}

    //! Construct a new connection from a configuration
    explicit TCPConnection(const TCPConfig &cfg) : _cfg{cfg} {}

    //! \name construction and destruction
    //! moving is allowed; copying is disallowed; default construction not possible

    //!@{
    ~TCPConnection();  //!< destructor sends a RST if the connection is still open
    TCPConnection() = delete;
    TCPConnection(TCPConnection &&other) = default;
    TCPConnection &operator=(TCPConnection &&other) = default;
    TCPConnection(const TCPConnection &other) = delete;
    TCPConnection &operator=(const TCPConnection &other) = delete;
    //!@}
};

#endif  // SPONGE_LIBSPONGE_TCP_FACTORED_HH
