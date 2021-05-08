#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#define MAX 80
#define PORT 8098
#define SA struct sockaddr
#define USER_MAX 20
// This system can store up to number of users
 
typedef struct
{
    char name[80];
} User;
// definition of the structure of a user storing
 
User list[USER_MAX];
// define a global array, type User, size is USER_MAX
 
char* filename = "/home/fiansyah/praktikum3/finak/akun.txt";
// user.txt need to create a file in your application directory





int flag=0;
 
 
// Log function
User login(char masuk[])
{
    char name[10];
    char password[10];
    User lg;
   // printf("enter name:");
   // scanf("%s",name);
   // strcat(name,":");
   // printf("enter password:");
   // scanf("%s",password);

    strcpy(lg.name , masuk);
    return lg;

}
 
// write txt file, there is a row for each user
void writeToFile(User u)
{
    FILE *fw = fopen(filename,"a+");
    fprintf(fw,u.name);
    fprintf(fw,"\n");
}
 
// determine whether the user exists
int exist(User u)
{
    int i;
    for(i=0; i<USER_MAX; i++)
    {
        if(0==strcmp(list[i].name ,u.name))
        {
            return 1;
        }
    }
    return -1;
}
 
//registered user
void registerUser(char regis[])
{
    char name[10];
    char password[10];
           

    User user;
    int i;
 
  //  printf("enter your name:");
   //  scanf("%s",name);
    // strcat(name,":");
    // determine whether there has been a registered user
    for(i=0; i<USER_MAX; i++)
    {
        if(0==strcmp(list[i].name ,regis))
        {
            printf("USER ALREADY EXIST");
            return;
        }
    }
 
   // printf("enter your password:");
   // scanf("%s",password);
    strcpy(user.name ,regis);
    writeToFile(user);
}
 
int menu()
{
    int choice;
    printf("1.login\n");
    printf("2.register\n");
    printf("#.exit\n");
    printf("enter your choice:");
    scanf("%d",&choice);
    return choice;
} 








// Function designed for chat between client and server.
void func(int sockfd)
{
    char buff[MAX];
    char masuk[MAX]="Login";
    char daftar[MAX]="Register";
    char username[10];
    char password[10];
    int n;
    // infinite loop for chat
    for (;;) {

        bzero(buff, MAX);
  
        // read the message from client and copy it in buffer
        read(sockfd, buff, sizeof(buff));
	if(strcmp(buff,daftar)==10){
		flag=1;
		char login[]="Masukkan:Username_Password\n";
       		write(sockfd, login, sizeof(login));
		continue;
	}
	else if(strcmp(buff,masuk)==10){
 		flag=2;
                char login[]="Masukkan:Username_Password\n";
                write(sockfd, login, sizeof(login));
                continue;

	}
	


///////////////////


    User test[USER_MAX];
    FILE *fp = fopen(filename,"r");
    int i=0;
    User u;
    if(NULL == fp)
    {
        printf("FILE NOT FOUND");
        return -1;
    }
    for(i=0; i<USER_MAX; i++)
    {
        char uname[10];
        char upassword[10];
        fscanf(fp,"%s",uname);
 
        strcpy(list[i].name , uname);
    }
 
 //   int choice = menu();
    if(flag==2)
    {
	flag=0;
	printf("buff=%s\n",buff);
        u=login(buff);
        if(1 == exist(u))
        {
            printf("success");
	 
        char login[]="Login Berhasil\n";
        write(sockfd, login, sizeof(login));
        continue;

        }
        else
        {
            printf("WRONG USER OR PASS");
	
        char login[]="Login Gagal\n";
        write(sockfd, login, sizeof(login));
        continue;

        }
	
    }
    else if(flag==1)
    {
	flag=0;
        
        registerUser(buff);
	
	char register1[]="Register Berhasil\n";
	write(sockfd, register1, sizeof(register1));
	continue;
    }
    else
    {
     
////////////////	

        // print buffer which contains the client contents
        printf("\nFrom client: %s\t To client : ", buff);
        bzero(buff, MAX);
        n = 0;
        // copy server message in the buffer
        while ((buff[n++] = getchar()) != '\n')
            ;
  
        // and send that buffer to client
        write(sockfd, buff, sizeof(buff));
  
        // if msg contains "Exit" then server exit and chat ended.
         if (strncmp("exit", buff, 4) == 0) {
            printf("Server Exit...\n");
            break;
         }
	
	}

    }
}
  
// Driver function
int main()
{
    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;
  
    // socket create and verification
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
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);
  
    // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");
  
    // Now server is ready to listen and verification
    if ((listen(sockfd, 5)) != 0) {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");
    len = sizeof(cli);
  
    // Accept the data packet from client and verification
    connfd = accept(sockfd, (SA*)&cli, &len);
    if (connfd < 0) {
        printf("server acccept failed...\n");
        exit(0);
    }
    else
        printf("server acccept the client...\n");
  

    // Function for chatting between client and server
    func(connfd);
  
    // After chatting close the socket
    close(sockfd);
}
