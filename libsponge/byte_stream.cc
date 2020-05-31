#include "byte_stream.hh"

#include <algorithm>
#include <iterator>
#include <stdexcept>

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`


using namespace std;

ByteStream::ByteStream(const size_t capacity)
    : _capacity(capacity),
      _eoi(false),
      _total_written(0),
      _total_popped(0),
      _error(false) { 
}

size_t ByteStream::write(const string &data) {
    if(input_ended()){
        set_error();
        return 0;
    }
    size_t prev_buffer_size = buffer_size();
    if(data.size() > remaining_capacity()){
        _data += data.substr(0, remaining_capacity());
    } else {
        _data += data;
    }
    size_t written = buffer_size() - prev_buffer_size;
    _total_written +=  written;
    return written;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
    size_t length = min(len, buffer_size());
    return _data.substr(0, length);
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) {
    if(len > 0 && eof()){
        set_error();
        return;
    }
    size_t length = min(len, buffer_size());
    _data = _data.substr(length, string::npos);
    _total_popped += length;
}

void ByteStream::end_input() {
    _eoi = true;
}

bool ByteStream::input_ended() const { return _eoi; }

size_t ByteStream::buffer_size() const { return _data.size(); }

bool ByteStream::buffer_empty() const { return buffer_size() == 0; }

bool ByteStream::eof() const { return input_ended() && buffer_empty(); }

size_t ByteStream::bytes_written() const { return _total_written; }

size_t ByteStream::bytes_read() const { return _total_popped; }

size_t ByteStream::remaining_capacity() const { return _capacity - buffer_size(); }
