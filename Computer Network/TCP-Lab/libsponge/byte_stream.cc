#include "byte_stream.hh"

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

ByteStream::ByteStream(const size_t capacity):_cap(capacity) { 
    _stream.resize(capacity);
}

size_t ByteStream::write(const string &data) {
    size_t writedDataNum = min(data.size(),_cap-_length);
    for(size_t i = 0;i<writedDataNum;i++){
        _stream[(_beginIndex+_length+i)%_cap] = data[i];
    }
    _length+=writedDataNum;
    _accumuWritten+=writedDataNum;
    return writedDataNum;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
    string result;
    size_t lenCur = min(len,_length);
    for(size_t i=0;i<lenCur;i++)
        result.push_back(_stream[(_beginIndex+i)%_cap]);
    return result;
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) { 
    size_t popsize = min(len,_length);
    _beginIndex = (_beginIndex+popsize)%_cap;
    _length-=popsize;
    _accumuRead+=popsize;
}

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len) {
    string result = peek_output(len);
    pop_output(len);
    return result;
}

void ByteStream::end_input() {_isEnd=true;}

bool ByteStream::input_ended() const { 
    if (_isEnd==1)
        return true;
    else
        return false;
}

size_t ByteStream::buffer_size() const { return _length; }

bool ByteStream::buffer_empty() const { 
    if (_length==0)
        return true;
    else
        return false;
}

bool ByteStream::eof() const {
    if (input_ended() && _length==0)
        return true;
    else
        return false;
}

size_t ByteStream::bytes_written() const { return _accumuWritten; }

size_t ByteStream::bytes_read() const { return _accumuRead; }

size_t ByteStream::remaining_capacity() const { return _cap-_length; }
