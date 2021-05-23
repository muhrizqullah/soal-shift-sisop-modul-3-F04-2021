#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#define MAX 80
#define PORT 20003
#define SA struct sockaddr



int flag=0;


void createNew(FILE *p,FILE *q){
        char c=fgetc(p);
        while(c!=EOF){
                fputc(c,q);
                c=fgetc(p);
        }
        
}




void func(int sockfd)
{
    char buff[MAX];
    int n;
    char login[]="Masukkan:Username_Password\n";
    
    for (;;) {
	
	if(flag==0){
	flag=1;
        char pilihan[]="";
printf("Menu\n1.Register\n2.Login\n3.Add\n4.download\n5.delete\n6.see\n7.find\n");
	}
	if(strcmp(buff,login)==0){
    char name[80];
    char password[80];
    
    printf("enter name:");
    scanf("%s",name);
    strcat(name,":");
    printf("enter password:");
    scanf("%s",password);
    strcat(name,password);	
    write(sockfd, name, sizeof(name));
	
	}
else  if(strcmp(buff,"Publisher:")==0){
        char name[80];
    char password[80];
  char pas[80];
  //  printf("enter name:");
    scanf("%s",name);
    printf("Tahun Publikasi:");
    scanf("%s",password);
    printf("Filepath:");
    scanf("%s",pas);
   


strcat(pas,",");
strcat(pas,name);
strcat(pas,",");
strcat(pas,password);

    write(sockfd, pas, sizeof(pas));

}
else if(strcmp(buff,"Gagal\n")==0){
	printf("%s",buff);
	flag=1;
	char tulis[80]="Downloads Gagal";
	write(sockfd, tulis, sizeof(tulis));
}
else if(flag==11){
//	printf("jj=%s\n",buff);
	flag=1;
        write(sockfd, buff, sizeof(buff));


}
else  if(flag==12){
//	printf("masuk  sini");
        FILE *o=fopen("/home/fiansyah/praktikum3/finak/client/data.txt","r");
	if(o==NULL){
	flag=1;
		continue;
	}
	char c=fgetc(o);
	while(c!=EOF){
		printf("%c",c);
		c=fgetc(o);
	}
	fclose(o);
        flag=1;
	remove("/home/fiansyah/praktikum3/finak/client/data.txt");
	char input[80];
	continue;
//	scanf("%s",input);
  //      write(sockfd, input, sizeof(input));

}



else{
        bzero(buff, sizeof(buff));
       // printf("Enter the string : ");
        n = 0;
        while ((buff[n++] = getchar()) != '\n')
            ;
	 if ((strncmp(buff, "exit", 4)) == 0) {
            printf("Client Exit...\n");
            break;
        }
//
	if(strcmp(buff,"see")==10){
		flag=12;
	}
	else if(strcmp(strtok(buff," "),"download")==0){
	char new[80];
	strcpy(new,strtok(NULL," "));
	strcat(buff," ");
	strcat(buff,new);
	strtok(buff,"\n");
	printf("buff=%s\n",buff);
	flag=11;
	}
        else if(strcmp(buff,"delete")==0){
        char new[80];
        strcpy(new,strtok(NULL," "));
	strcat(buff," ");
        strcat(buff,new);
	strtok(buff,"\n");
        flag=11;
        }
	else if(strcmp(buff,"find")==0){
        char new[80];
        strcpy(new,strtok(NULL," "));
        strcat(buff," ");
        strcat(buff,new);
        strtok(buff,"\n");

        flag=12;
        }
	
	write(sockfd, buff, sizeof(buff));
}
        bzero(buff, sizeof(buff));
        read(sockfd, buff, sizeof(buff));
	if(flag!=11&&flag!=12)	printf("%s",buff);
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
