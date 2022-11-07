#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024


 
int main(int argc, char *argv[])
{
    int sock;
    char message[BUF_SIZE];
    int str_len, recv_len, recv_cnt;
    struct sockaddr_in serv_addr;
 
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv_addr.sin_port = htons(7777);

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        printf("connect error");
    else
        printf("Connected success");

 
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
    close(sock);
    return 0;
}
 