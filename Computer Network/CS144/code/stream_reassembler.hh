#ifndef SPONGE_LIBSPONGE_STREAM_REASSEMBLER_HH
#define SPONGE_LIBSPONGE_STREAM_REASSEMBLER_HH

#include "byte_stream.hh"

#include <cstdint>
#include <string>

#include <memory>
#include <queue>
#include <list>
#include <vector>
#include <unordered_set>


//! \brief A class that assembles a series of excerpts from a byte stream (possibly out of order,
//! possibly overlapping) into an in-order byte stream.
class StreamReassembler {
  private:
    // Your code here -- add private members as necessary.

   
    ByteStream _output;  //!< The reassembled in-order byte stream 储存按序排列的字节流 这里边肯定是连续的数据
    const size_t _capacity;    //!< The maximum number of bytes

    std::string _storage; //这个是lab1文档中的那个图中的width:capacity,储存连续的数据或者未按序到达的数据。
    std::vector<bool> _storageEmpty; // 标记storage的某一位是否为空 这个类的核心主要是维护这两个数据结构

    size_t _unassembledbytes = 0; //储存的没有被组装的字节数量
    size_t _firstUnassembled = 0; //字节流中第一个没有连续装配的序号
    size_t _firstUnread=0; //字节流中第一个unread的序号
    size_t _EOFindex = 0; //储存eof在字节流中的序号

    size_t _lastReassembledByte = 0; 
    //最后一个放入到storage中的数据的序号的后一位，可能是乱序到达的数据的最后一个序号，就是那个图中红色块的最后一个数据

  public:
    //! \brief Construct a `StreamReassembler` that will store up to `capacity` bytes.
    //! \note This capacity limits both the bytes that have been reassembled,
    //! and those that have not yet been reassembled.
    StreamReassembler(const size_t capacity);

    //! \brief Receive a substring and write any newly contiguous bytes into the stream.
    //!
    //! The StreamReassembler will stay within the memory limits of the `capacity`.
    //! Bytes that would exceed the capacity are silently discarded. 超出容量的数据将被丢弃 且应该处理重复序号的情况
    //!
    //! \param data the substring
    //! \param index indicates the index (place in sequence) of the first byte in `data` 应该就是序号
    //! \param eof the last byte of `data` will be the last byte in the entire stream 
    void push_substring(const std::string &data, const uint64_t index, const bool eof);

    //! \name Access the reassembled byte stream
    //!@{
    const ByteStream &stream_out() const { return _output; }
    ByteStream &stream_out() { return _output; }
    //!@}

    //! The number of bytes in the substrings stored but not yet reassembled
    //!
    //! \note If the byte at a particular index has been pushed more than once, it
    //! should only be counted once for the purpose of this function.
    size_t unassembled_bytes() const;

    size_t readLastreassembledByte()const{return _lastReassembledByte;}
    size_t read_firstUnassembled()const {return _firstUnassembled;} //读取第一个待加载的序号

    //! \brief Is the internal state empty (other than the output stream)?
    //! \returns `true` if no substrings are waiting to be assembled
    bool empty() const;
};

#endif  // SPONGE_LIBSPONGE_STREAM_REASSEMBLER_HH
