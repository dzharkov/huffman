#include <cstdio>
#include <unistd.h>
#include "huffman.h"

void display_usage() {
    printf("Usage: arg [-c|-d] -i <filename> -o <filename>\n");
}

int main(int argc, char * argv[]) {
    
    int next_opt = 0;
    bool is_compress = true;

    char* input_filename = NULL;
    char* output_filename = NULL;

    while ((next_opt = getopt(argc, argv, "cdi:o:")) != -1) {
        switch( next_opt ) {
            case 'c':
                is_compress = true;
                break;
            case 'd':
                is_compress = false;
                break;
            case 'i':
                input_filename = optarg;
                break;
            case 'o':
                output_filename = optarg;
                break;
            case '?':
                display_usage();
                return 1;
        }
    }

    if (input_filename == NULL || output_filename == NULL) {
        printf("Input/Output filenames should be specified\n");
        display_usage();
        return 1;
    }
    
    if (is_compress) {
        huffman_compress(input_filename, output_filename);
    } else {
        huffman_decompress(input_filename, output_filename);
    }
    
    return 0;
}
