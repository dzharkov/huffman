#include "bstream.h"

bool ibstream::eof() {
    fill_read_bits();
    return is_eof && read_bits.empty();
}
        
bool ibstream::read_bit() {
    fill_read_bits();
            
    if (read_bits.empty()) {
        return false;
    }
            
    bool result = read_bits.front();
    read_bits.pop();
    return result;
}

void ibstream::fill_read_bits() {
    if (!read_bits.empty()) {
        return;
    }
    if (is_eof) {
        return;
    }

    while (buf.size() < 3) {
        byte_t next = inp_stream.get();

        if (!inp_stream.good()) {
             is_eof = true;
             break;   
        } else {
            buf.push(next);
        }
    }

    if (buf.size() < 2) {
        return;
    }
    
    size_t means_byte_count = byte_length;
    byte_t last = buf.front();
    buf.pop();
    
    //last byte of file is number of meaningful bits in the previous byte
    if (buf.size() == 1) {
        
        size_t last_count = buf.front();
        buf.pop();

        means_byte_count = last_count;
    }
    
    for (size_t i = 0; i < means_byte_count; i++) {
        size_t shift = byte_length - i - 1;
        
        read_bits.push((last & (1 << shift)) > 0 ? 1 : 0);
    }
}


void obstream::write_byte(byte_t b) {
    if (is_closed) {
        return;
    }

    outp_stream.put(b);
}

void obstream::write_current_buffer() { 
    write_byte(buffer);
    buffer = 0;
}

void obstream::write_bit(bool bit) {
    if (is_closed) {
        return;
    }
    
    if (buffered_bits_count == 8) {
        write_current_buffer(); 
        buffered_bits_count = 0;
    }

    if (bit) {
        buffer |= 1 << (byte_length - buffered_bits_count - 1);
    }

    buffered_bits_count++;
}

void obstream::close() {
    if (is_closed) {
        return;
    }

    write_current_buffer();
    write_byte(buffered_bits_count);
    
    is_closed = true;
}
