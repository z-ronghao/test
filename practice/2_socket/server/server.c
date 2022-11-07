#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#define BUF_SIZE 1024

int main()
{
    int serv_sock, clnt_sock;
    char message[BUF_SIZE];
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size = sizeof(clnt_addr);
    int str_len;

    serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv_addr.sin_port = htons(7777);

    if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) 
    {
        printf("bind error\n");
        return -1;
    }
    listen(serv_sock, 5);
    if (listen(serv_sock, 5) == -1) 
    {   
        printf("listen error\n");
        return -1;
    }

    printf("======Waiting for client's request======\n");

    while (1) 
    {
        
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
        if (clnt_sock == -1) 
        {
            printf("accept Error");
            return -1;
        } else 
        {
            printf("Connecte Success \n");
        }
        char *add ;
        snprintf(add, 20, "%s%s",inet_ntoa(clnt_addr.sin_addr), ": ");
        
        while ((str_len = read(clnt_sock, message, BUF_SIZE)) != 0) 
        {
            printf("client send : %s",  message);
            
            write(clnt_sock, add, strlen(add));
            write(clnt_sock, message, str_len);
            //message[str_len] = '\0';
            
            memset(message, 0, str_len);
        }

            // close the socket of the client
        close(clnt_sock);
    }

    // close the socket of the server
    close(serv_sock);

    return 0;
}