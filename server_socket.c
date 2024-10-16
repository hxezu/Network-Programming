#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define PORT 9000
#define BUFF_SIZE 1024

int main(int argc, char **argv){
    int server_socket = 0;
    int client_socket = 0;
    int client_addr_size = 0;
    int ret = 0;
    
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    
    //char buff[BUFF_SIZE+5];
    char buff_rcv[BUFF_SIZE];
    char buff_snd[BUFF_SIZE];

    //memset(&server_addr, 0x00, sizeof(server_addr));
    //memset(&client_addr, 0x00, sizeof(client_addr));

    server_socket = socket(PF_INET, SOCK_STREAM, 0);
    if(server_socket == -1){
        printf("server socket 생성 실패\n");
        exit(1);
    }

    //서버 주소 초기화
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    //소켓 바인드
    if(bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
    {
        printf("bind() 실행 에러 \n");
        exit(1);
    }

    //클라이언트 연결 대시
    if(listen(server_socket, 5) == -1 ){
        printf("listen() 실행 실패\n");
        exit(1);
    }
    
    while(1){
        client_addr_size = sizeof(client_addr);
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_size);

        if(client_socket == -1 ){
            printf("클라이언트 연결 수락 실패\n");
            exit(1);
        }

        while(1){
            ret = read(client_socket, buff_rcv, BUFF_SIZE);
            if(ret <= 0){
                printf("read error\n");
                break;
            }
            
            buff_rcv[ret] = '\0';
            printf("클라이언트로부터 받은 메시지: %s\n", buff_rcv);

            // if((strlen(buff_rcv)==1)&&(buff_rcv[0]=='q')){
            //     printf("클라이언트 종료 요청\n");
            //     break;
            // }

            if (strcmp(buff_rcv, "bye") == 0) {
                printf("클라이언트 종료 요청\n");
                break;
            }
            
            //서버 답장
            printf("서버 답장 입력 : ");
            scanf("%s", buff_snd);

            ret = write(client_socket, buff_snd, strlen(buff_snd)+1);
            if(ret<0){
                printf("write error\n");
                break;
            }
        }
        close(client_socket);
        printf("클라이언트 소켓 종료, 다음 클라이언트 대기 중...\n");

    }
    
    close(server_socket); // 서버 소켓 종료
    return 0;
    
}