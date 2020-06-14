// export CTEST_OUTPUT_ON_FAILURE=1 // this is to guarantee cout on standard output


#include "wrapping_integers.hh"
#include <math.h>
#include <iostream>

// Dummy implementation of a 32-bit wrapping integer

// For Lab 2, please replace with a real implementation that passes the
// automated checks run by `make check_lab2`.

using namespace std;

//! Transform an "absolute" 64-bit sequence number (zero-indexed) into a WrappingInt32
//! \param n The input absolute 64-bit sequence number
//! \param isn The initial sequence number
WrappingInt32 wrap(uint64_t n, WrappingInt32 isn) {
    uint64_t base = pow(2, 32);
    uint64_t absolute = n;

    uint64_t remainder = absolute % base;
    uint64_t seqno = remainder + isn.raw_value();
    seqno = seqno % base;

    return WrappingInt32{static_cast<uint32_t>(seqno)};
}

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
    uint64_t base = pow(2, 32);
    uint32_t remainder = n - isn;
    uint64_t tail = remainder;

    if(tail > checkpoint){
        return tail;
    }

    uint64_t body = 0;
    while(body + tail <= UINT64_MAX - base){
        uint64_t absolute = body + tail;
        if(absolute <= checkpoint && absolute + base >= checkpoint){
            if(checkpoint - absolute <= absolute + base - checkpoint){
                break;
            } else{
                body += base;
                break;
            }
        }
        body += base;
    }
    return body + tail;
}
