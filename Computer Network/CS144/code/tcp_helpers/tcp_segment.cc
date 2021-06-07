#include "tcp_segment.hh"

#include "parser.hh"
#include "util.hh"

#include <variant>

using namespace std;

//! \param[in] buffer string/Buffer to be parsed
//! \param[in] datagram_layer_checksum pseudo-checksum from the lower-layer protocol
ParseResult TCPSegment::parse(const Buffer buffer, const uint32_t datagram_layer_checksum) {
    InternetChecksum check(datagram_layer_checksum);
    check.add(buffer);
    if (check.value()) { //通过校验和检查收到的数据是否正确
        return ParseResult::BadChecksum;
    }

    NetParser p{buffer}; //如果正确，则对buffer中的数据进行解析
    _header.parse(p);  //解析头部，把头部数据放入到TCPHeader类的_header对象中，这个过程中对buffer的数据有改变，丢弃了整个header 
    _payload = p.buffer(); //经过上一步对header的丢弃后，就可以直接把buffer当成payload了
    return p.get_error();
}

size_t TCPSegment::length_in_sequence_space() const {
    return payload().str().size() + (header().syn ? 1 : 0) + (header().fin ? 1 : 0);
}

//! \param[in] datagram_layer_checksum pseudo-checksum from the lower-layer protocol
BufferList TCPSegment::serialize(const uint32_t datagram_layer_checksum) const {
    TCPHeader header_out = _header;
    header_out.cksum = 0; //首先设置校验和位为0

    // calculate checksum -- taken over entire segment 计算校验和
    InternetChecksum check(datagram_layer_checksum);
    check.add(header_out.serialize());
    check.add(_payload);
    header_out.cksum = check.value(); 
    //把header和payload中的所有数据都加起来，然后溢出位回卷，最后取补码得到校验和。在计算机网络自顶向下有表述

    BufferList ret;
    ret.append(header_out.serialize());
    ret.append(_payload);

    return ret;
}
