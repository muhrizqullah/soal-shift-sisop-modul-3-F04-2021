#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#define MAX 80
#define PORT 8098
#define SA struct sockaddr

int flag=0;

void func(int sockfd)
{
    char buff[MAX];
    int n;
    char login[]="Masukkan:Username_Password\n";
    for (;;) {
	
	if(flag==0){
	flag=1;
        char pilihan[]="";
        printf("Menu\n1.Register\n2.Login\n");
	}
	if(strcmp(buff,login)==0){
    char name[10];
    char password[10];
    
    printf("enter name:");
    scanf("%s",name);
    strcat(name,":");
    printf("enter password:");
    scanf("%s",password);
    strcat(name,password);
   printf("nama:%s\n",name);	
    write(sockfd, name, sizeof(name));
	
	}

else{
        bzero(buff, sizeof(buff));
        printf("Enter the string : ");
        n = 0;
        while ((buff[n++] = getchar()) != '\n')
            ;
        write(sockfd, buff, sizeof(buff));
}
        bzero(buff, sizeof(buff));
        read(sockfd, buff, sizeof(buff));
        printf("From Server : %s", buff);
        if ((strncmp(buff, "exit", 4)) == 0) {
            printf("Client Exit...\n");
            break;
        }
    }
}
  
int main()
{
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;
  
    // socket create and varification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));
  
    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);
  
    // connect the client socket to server socket
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
        printf("connected to the server..\n");
  
    // function for chat
    func(sockfd);
  
    // close the socket
    close(sockfd);
}
