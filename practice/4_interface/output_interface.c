#include "output_interface.h"



int out_file(uint8_t* stream, uint64_t stream_length)
{
    FILE* fp;
    fp = fopen("./outfile","w");
    long wlength = fwrite(stream, sizeof(uint8_t), stream_length, fp);
    printf("write size =  %ld\n", wlength);
    fclose(fp);
    return 0;
}

int save_ram(uint8_t* stream, uint64_t stream_length)
{
    uint8_t * buf = (uint8_t *)malloc(stream_length);
    memcpy(buf, stream, stream_length);

    return 0;
}

int send_socket(uint8_t* stream, uint64_t stream_length)
{
    int sock,str_len, recv_len, recv_cnt;
    char message[BUF_SIZE];
    struct sockaddr_in serv_addr;
    char str_addr[20], str_port[4];
    printf("pleaese input <inet_addr> and <port>\n");
    gets(str_addr);
    gets(str_port);

    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(str_addr);
	serv_addr.sin_port = htons(atoi(str_port));

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        printf("connect error");
    else
        printf("Connected success");

    /*              send stream to server
    while (1)
    {
        fputs("send message(Q to quit):", stdout);
        fgets(message, BUF_SIZE, stdin);
         
        if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
            break;
 
        str_len = write(sock, message, strlen(message));
        recv_len = 0;
        while (recv_len < str_len)
        {
            recv_cnt = read(sock, &message[recv_len], BUF_SIZE - 1);
            if (recv_cnt == -1)
                printf("read error!");
            recv_len += recv_cnt;
        }
 
        message[recv_len] = 0;
        printf("%s", message);
    }
    */
    close(sock);
    return 0;
}

int output_interface(uint8_t *stream,uint64_t stream_length, output_type type)
{

    switch(type)
    {
        case OUT_FILE:
            out_file(stream, stream_length);
            break;
        case SAVE_RAM:
            save_ram(stream, stream_length);
            break;
        case SEND_SOCKET:
            send_socket(stream, stream_length);
            break;
        default:
            printf("please set output_type <OUT_FILE> or <SAVE_RAM> or <SEND_SOCKET>\n");
            return 0;
    }

}

