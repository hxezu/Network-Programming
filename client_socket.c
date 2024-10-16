#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define BUFF_SIZE 1024
#define PORT 9000

int main(int argc, char **argv){
    int client_socket;
    int ret = 0;

    struct sockaddr_in server_addr;
    char buff[BUFF_SIZE+5];

    // 클라이언트 소켓 생성
    client_socket = socket(PF_INET, SOCK_STREAM, 0);
    if(client_socket == -1){
        printf("client socket 생성 실패\n");
        exit(1);
    }

    // 서버 주소 초기화
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // 서버에 접속 시도
    if(connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr))== -1){
        printf("접속 실패\n");
        exit(1);
    }

    while(1){
        // 클라이언트에서 서버로 메시지 전송
        printf("메시지 입력: ");
        scanf("%s", buff);

        ret = write(client_socket, buff, strlen(buff) + 1);
        if(ret<0){
            printf("write error \n");
            break;
        }

        // "bye" 입력 시 클라이언트 종료
        if (strcmp(buff, "bye") == 0) {
            printf("클라이언트 종료 요청\n");
            break;
        }

        // 서버로부터 답장 수신
        memset(buff, 0, sizeof(buff));
        ret = read(client_socket, buff, BUFF_SIZE);
        if(ret < 0){
            printf("read error\n");
            break;
        }
        printf("서버로부터 답장 : %s\n", buff);
    }
    
    sleep(1);
    close(client_socket);
    return 0;
}