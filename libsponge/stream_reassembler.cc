#include "stream_reassembler.hh"
#include <stdexcept>
#include <algorithm>
#include <iostream>

// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`

using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity) 
: _unassembled_bytes(0),
 _curr_pos(0),
 _eof_index(capacity),
 _cache(capacity, make_pair(false, '\0')),
 _output(capacity),
 _capacity(capacity)
 {}

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
    if(index >= _eof_index){
        return;
    }

    size_t j = index;

    for(size_t i = 0; i < data.size(); ++i){
        if(i == _eof_index){
            break;
        }
        if(!_cache[j].first){
            _cache[j].first = true;
            _cache[j].second = data[i];
            ++_unassembled_bytes;
        }
        ++j;
    }

    if(eof){
        _eof_index = std::min(_capacity, index + data.size());
    }

    std::string to_write = "";
    for(size_t i = _curr_pos; i < _eof_index; ++i){
        if(!_cache[i].first){
            break;
        }
        to_write += _cache[i].second;
    }

    if(to_write.size() != 0){
        _unassembled_bytes -= to_write.size();
        _curr_pos += to_write.size();
        _output.write(move(to_write));
    }

    if(_curr_pos == _eof_index){
        _output.end_input();
    }
}

size_t StreamReassembler::unassembled_bytes() const { 
    return _unassembled_bytes;
}

bool StreamReassembler::empty() const { return unassembled_bytes() == 0; }
