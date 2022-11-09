#ifndef _OUTPUT_INTERFACE_H
#define _OUTPUT_INTERFACE_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MAXSIZE 4194304
#define BUF_SIZE 1024


typedef enum output_type_
{
    OUT_FILE = 0,
    SAVE_RAM,
    SEND_SOCKET
}output_type;


int out_file(uint8_t* stream, uint64_t stream_length);
int save_ram(uint8_t* stream, uint64_t stream_length);
int send_socket(uint8_t* stream, uint64_t stream_length);

int output_interface(uint8_t* stream,uint64_t stream_length, output_type type);

#endif
