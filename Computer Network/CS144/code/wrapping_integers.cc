#include "wrapping_integers.hh"

// Dummy implementation of a 32-bit wrapping integer

// For Lab 2, please replace with a real implementation that passes the
// automated checks run by `make check_lab2`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}
#include <iostream>
using namespace std;

//! Transform an "absolute" 64-bit sequence number (zero-indexed) into a WrappingInt32
//! \param n The input absolute 64-bit sequence number
//! \param isn The initial sequence number
WrappingInt32 wrap(uint64_t n, WrappingInt32 isn) {
    // uint32_t ss = n&static_cast<uint64_t>(0xffffffff);
    // return WrappingInt32{ss+isn.raw_value()};
    return isn+n; //在计算结果上，上下这两个可能是相同的，然后上边的优势可能是更快？或者是代码习惯更好？程序更可控？
}

//将压缩的32位序列号转化成非压缩的绝对序列号64位数
//! Transform a WrappingInt32 into an "absolute" 64-bit sequence number (zero-indexed)
//! \param n The relative sequence number
//! \param isn The initial sequence number
//! \param checkpoint A recent absolute 64-bit sequence number
//! \returns the 64-bit sequence number that wraps to `n` and is closest to `checkpoint`
//!
//! \note Each of the two streams of the TCP connection has its own ISN. One stream
//! runs from the local TCPSender to the remote TCPReceiver and has one ISN,
//! and the other stream runs from the remote TCPSender to the local TCPReceiver and
//! has a different ISN.
uint64_t unwrap(WrappingInt32 n, WrappingInt32 isn, uint64_t checkpoint) {
    //最终输出的uint64_t在[checkpoint-2^32/2,checkpoint+2^32/2]范围内
    uint32_t Difference;
    if(n.raw_value()>=isn.raw_value()){
        Difference = n.raw_value()-isn.raw_value();
    }else{
        Difference = n.raw_value()+(~isn.raw_value())+1;
    }
    //然后实际的绝对序号可能等于2^32+Difference 或2^33+Difference 或 2^34+Difference等，
    //需要在这里边找到最靠近checkpoint的那个值作为最终值
    
    uint64_t pow2_32 =static_cast<uint64_t>(1)<<32;
    uint64_t divide = checkpoint/pow2_32;
    uint64_t result1 = divide*pow2_32+Difference; //这里的result与checkpoint之间的差距不超过2^32
    uint64_t result2 = (divide+1)*pow2_32+Difference;
    uint64_t result3 = divide==0?UINT64_MAX:(divide-1)*pow2_32+Difference;
    uint64_t abs1 = result1>checkpoint?result1-checkpoint:checkpoint-result1;
    uint64_t abs2 = result2>checkpoint?result2-checkpoint:checkpoint-result2;
    uint64_t abs3 = result3>checkpoint?result3-checkpoint:checkpoint-result3;
    
    uint64_t result;
    if(abs1<=abs2 && abs1<=abs3)
        result = result1;
    else if (abs2<=abs1 && abs2<=abs3)
        result = result2;
    else if (abs3<=abs1 && abs3<=abs2)
        result = result3;
    else
        throw exception();
    return result; 
}
//