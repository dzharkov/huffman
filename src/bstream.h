#include <iostream>
#include <queue>

#ifndef BSTREAM_H
#define BSTREAM_H

typedef unsigned char byte_t;
const size_t byte_length =  8;

class ibstream {
    public:
        ibstream(std::istream &inp_stream_) : inp_stream(inp_stream_), is_eof(false) {
        }

        ~ibstream() {
        }

        bool eof();
        bool read_bit();
    private:
        std::istream & inp_stream;
        bool is_eof;
        
        std::queue< byte_t > buf;
        std::queue< bool > read_bits;
        
        void fill_read_bits(); 

        ibstream &operator=(ibstream const &a);
        ibstream(ibstream const &a);
};

class obstream {
    public:
        obstream(std::ostream & outp_stream_) : outp_stream(outp_stream_), is_closed(false), buffer(0), buffered_bits_count(0) {
        }
        
        ~obstream() {
            close();
        }
        
        void write_bit(bool bit);
        void close();
    private:
        std::ostream & outp_stream;
        bool is_closed;
        byte_t buffer;
        byte_t buffered_bits_count;
        
        void write_byte(byte_t b); 
        void write_current_buffer();
        obstream &operator=(obstream const &a);
        obstream(obstream const &a);
};

#endif
