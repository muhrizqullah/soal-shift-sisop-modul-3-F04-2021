#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#define MAX 80
#define PORT 20003
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
 
char* filename = "/home/fiansyah/praktikum3/finak/server/akun.txt";
char* filename1 = "/home/fiansyah/praktikum3/finak/server/files.tsv";
char* serverpath="/home/fiansyah/praktikum3/finak/client/";

// user.txt need to create a file in your application directory



int flag=0;
int loginer=0; 
char userlogin[80];
 
// Log function
User login(char masuk[])
{
printf("There is user login with username & password :%s\n",masuk);
    char name[80];
    char password[80];
    User lg;
    strcpy(lg.name , masuk);
    return lg;

}
 
// write txt file, there is a row for each user
void writeToFile(User u,int sockfd)
{
printf("There is user register with username & password :%s\n",u.name);

    FILE *fw = fopen(filename,"a+");
    fprintf(fw,u.name);
    fprintf(fw,"\n");
    fclose(fw);
     char register1[]="Register Berhasil\n";
     write(sockfd, register1, sizeof(register1));

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
void registerUser(char regis[],int sockfd)
{
    char name[80];
    char password[80];
    int lenght=strlen(regis);
    char baru[lenght];
    strcpy(baru,regis); 

    User user;
    int i;
    for(i=0; i<USER_MAX; i++)
    {
	strtok(baru,":");
        int nilai=strncmp(strtok(list[i].name,":"),baru,strlen(baru));
	if(nilai==0)
        {
            char register1[]="Register Gagal\n";
            write(sockfd, register1, sizeof(register1));

            return;
        }
    }
    strcpy(user.name ,regis);
    writeToFile(user,sockfd);
}
 

int cekdownload(char regis[],int sockfd)
{
    
    User user;
    int i;
   
 
    for(i=0; i<USER_MAX; i++)
    {

	int nilai=strcmp(list[i].name,regis); 
        if(nilai==0&&strlen(list[i].name)==strlen(regis))
        {
        printf("DITEMUKAN\n");
            return i+1;
        }
    }
        printf("TIDAK DITEMUKAN\n");
	flag=0;
        char login1[]="Gagal\n";
        write(sockfd, login1, sizeof(login1));

    return 0;

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


void createNew(FILE *p,FILE *q){         
	char c=fgetc(p);
	while(c!=EOF){
		fputc(c,q);
		c=fgetc(p);
	}
	
}

  void delete(int lno) 
  {
	int max=1000;
        int ctr = 0;
        char ch;
        FILE *fptr1, *fptr2;
		char fname[]="/home/fiansyah/praktikum3/finak/server/files.tsv";
        char str[max], temp[] = "temp.txt";
		printf("-----------------------------------------\n");
        fptr1 = fopen(fname, "r");
        if (!fptr1) 
		{
                printf(" File tidak ada!!\n");
                return 0;
        }
        fptr2 = fopen(temp, "w"); // open the temporary file in write mode 
        if (!fptr2) 
		{
                printf("File dapat dibuka\n");
                fclose(fptr1);
                return 0;
        }
  
		lno++;
        // copy all contents to the temporary file except the specific line
        while (!feof(fptr1)) 
        {
            strcpy(str, "\0");
            fgets(str, max, fptr1);
            if (!feof(fptr1)) 
            {
                ctr++;
                /* skip the line at given line number */
                if (ctr != lno) 
                {
                    fprintf(fptr2, "%s", str);
                }
            }
        }
        fclose(fptr1);
        fclose(fptr2);
        remove(fname);  		// remove the original file 
        rename(temp, fname); 	// rename the temporary file to original name
/*------ Read the file ----------------*/
   fptr1=fopen(fname,"r"); 
            ch=fgetc(fptr1); 
          printf("Isi file dari  %s adalah : \n",fname); 
          while(ch!=EOF) 
            { 
                printf("%c",ch); 
                 ch=fgetc(fptr1); 
            }
        fclose(fptr1);
/*------- End of reading ---------------*/
        return 0;

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
  	mkdir("/home/fiansyah/praktikum3/finak/server/file",0777);
        // read the message from client and copy it in buffer
         read(sockfd, buff, sizeof(buff));
	if(flag==6){
        flag=0;
         FILE *p,*q;
	chdir("/home/fiansyah/praktikum3/finak/server/file");
        p=fopen(buff,"r");
	if(p!=NULL){
	chdir("/home/fiansyah/praktikum3/finak/client");
        q=fopen(buff,"a+");
        createNew(p,q);
        fclose(p);
        fclose(q);
        char login1[]="\nBerhasil download\n";
        write(sockfd, login1, sizeof(login1));
        continue;
	}
	else {
	char login1[]="\nGagal download\n";
        write(sockfd, login1, sizeof(login1));
	}
  	}
	if(flag==8){
        flag=0;
         FILE *p,*q;
        chdir("/home/fiansyah/praktikum3/finak/server/file");
        p=fopen(buff,"r");
        if(p!=NULL){
	char baru[]="old-";
        q=fopen(strcat(baru,buff),"a+");
        createNew(p,q);
        fclose(p);
        fclose(q);
	remove(buff);
	FILE *r=fopen("/home/fiansyah/praktikum3/finak/server/running.log","a+");
	fprintf(r,"Hapus:");
	fprintf(r,buff);
	fprintf(r,"(");
	fprintf(r,userlogin);
	fprintf(r,")\n");
	fclose(r);
        char login1[]="\nBerhasil delete\n";
        write(sockfd, login1, sizeof(login1));
        continue;
        }
        else {
        char login1[]="\nGagal delete\n";
        write(sockfd, login1, sizeof(login1));
        }
        }


	else if(strcmp(buff,daftar)==10){
		flag=1;
		char login[]="Masukkan:Username_Password\n";
       		write(sockfd, login, sizeof(login));
		continue;
	}
	else if(strcmp(buff,masuk)==10){
 		flag=2;
                char login1[]="Masukkan:Username_Password\n";
                write(sockfd, login1, sizeof(login1));
                continue;

	}
	else if(strcmp(buff,"Add")==10){
                flag=4;
		printf("Add\n");
                char login1[]="Publisher:";
                write(sockfd, login1, sizeof(login1));
                continue;
        }
	 else if(strcmp(buff,"see")==10){
                flag=9;
        	printf("see\n");
		FILE *j=fopen(filename1,"r");
                
        }
	else if(strcmp(buff,"exit")==32){
		break;
	}

	 char* cek=strtok(buff," ");
         if(strcmp(cek,"download")==0){
                flag=5;
                cek=strtok(NULL," ");
        }
	 if(strcmp(cek,"delete")==0){
                flag=7;
                cek=strtok(NULL," ");
        }
	if(strcmp(cek,"find")==0){
		printf("find\n");
                flag=10;
                cek=strtok(NULL," ");
        }



    User test[USER_MAX];
    FILE *fp;
    if(flag==5||flag==7||flag==9){
	fp = fopen(filename1,"a+");
    }
    else{
	fp = fopen(filename,"a+");
    }
    int i=0;
    User u;
    if(NULL == fp)
    {

        printf("FILE NOT FOUND");
      
    }
    for(i=0; i<USER_MAX; i++)
    {
        char uname[80];
        char upassword[80];
        fscanf(fp,"%s",uname);

	if(flag==5||flag==7){
   char* token1 = strtok(uname, ",");
   char* token = strtok(token1, "/");
   char* save[10];
        int b=0;
    // Keep printing tokens while one of the
    // delimiters present in str[].
    while (token != NULL) {
       token=strtok(NULL, "/");
        save[b]=token;
       
        b++;
    }

//	printf("token:%s\n",save[b-2]);
	if(save[0]==NULL)strcpy(list[i].name ,"kosong");
	else strcpy(list[i].name , save[b-2]);
	}
	else strcpy(list[i].name , uname);
   
    }
	fclose(fp);

    if(flag==2)
    {
	
//	printf("buff login:%s\n",buff);
	flag=0;
        u=login(buff);
	if(1 == loginer)
        {
//            printf("success");
        char login[]="Log out dulu!!\n";
        write(sockfd, login, sizeof(login));
        continue;

        }
        else if(1 == exist(u))
        {
//            printf("success");
	strcpy(userlogin,u.name);
	printf("User Login sekarang:%s\n",userlogin);
	loginer=1;
        char login[]="Login Berhasil\n";
        write(sockfd, login, sizeof(login));
        continue;

        }
        else
        {
  //          printf("WRONG USER OR PASS");
	
        char login[]="Login Gagal\n";
        write(sockfd, login, sizeof(login));
        continue;

        }
	
    }
    else if(flag==1)
    {
	flag=0;
        
        registerUser(buff,sockfd);
	
	continue;
    }
else if(flag==4)
    {

//printf("flag=4\n");
        flag=0;

          // receive file name
	FILE *p,*q,*r;
	char tulis[strlen(buff)];
	strcpy(tulis,buff);
 	char *publisher;
        publisher=strtok(buff,",");
	p=fopen(publisher,"r");
	if (p == NULL||loginer==0){
        char login[]="\nFile Failed opened!\n";
        write(sockfd, login, sizeof(login));
        continue;

        }
        else{
	    FILE *ftsv = fopen(filename1,"a+");
            fprintf(ftsv,tulis);
	    fprintf(ftsv,"\n");
             fclose(ftsv);

	}

	printf("\nFile Name Received: %s\n", publisher);
       chdir("/home/fiansyah/praktikum3/finak/server/file");
   char* token = strtok(publisher, "/");
   char* save[10];
  i=0;
    
    while (token != NULL) {
       token=strtok(NULL, "/");
	save[i]=token;
	i++;
    }
	q=fopen(save[i-2],"a+");
        if (p == NULL){
        char login[]="\nFile Failed opened!\n";
        write(sockfd, login, sizeof(login));
        continue;

	}
        else{
	    createNew(p,q);
          //  printf("\nFile Successfully opened!\n");
	    fclose(p);
	    fclose(q);

	FILE *r=fopen("/home/fiansyah/praktikum3/finak/server/running.log","a+");
	fprintf(r,"Tambah:");
	fprintf(r,save[i-2]);
	fprintf(r,"(");
	fprintf(r,userlogin);
	fprintf(r,")\n");
	fclose(r);
	    char login[]="\nFile Successfully opened!\n";
            write(sockfd, login, sizeof(login));
            continue;
        }
   }
  else if(flag==5){
	flag=6;
	printf("cek:%s\n",cek);
	if(cekdownload(cek,sockfd)>=1){     
		if(loginer==0){
	printf("TIDAK DITEMUKAN\n");
        flag=0;
        char login1[]="Gagal\n";
        write(sockfd, login1, sizeof(login1));
	continue;
		}
          
               write(sockfd, cek, sizeof(buff));
               continue;
	}
	else {
	     flag=0;
               continue;
        }

	
  }
   else if(flag==7){
        flag=8;
	int value=cekdownload(cek,sockfd);
        if(value>=1){
    if(loginer==0){
        printf("TIDAK DITEMUKAN\n");
        flag=0;
        char login1[]="Gagal\n";
        write(sockfd, login1, sizeof(login1));
        continue;
                }
	delete(value-1);
               write(sockfd, cek, sizeof(buff));
               continue;
        }
        else {
              flag=0;
               continue;
        }


  }
  else if(flag==9){
	int flag1=0;
	flag=0;
    char uname[80];
    char kosong[]=" ";
    FILE *j=fopen(filename1,"r");
    

    int loop=20;
    for(loop=0;loop<20;loop++)
    {
        fscanf(j,"%s",uname);
//	printf("uname=%d\n loop=%d\n",strcmp(uname,""),loop);
        if((strcmp(uname,"")==32&&loop==0)||loginer==0){
	flag=0;
	flag1=1;
	char* berhasil="";
        write(sockfd, berhasil, sizeof(berhasil));
        break;
	}
	else ;
	if(strcmp(uname,kosong)==0)break;
	char* path=strtok(uname,",");
	char path1[strlen(path)];
	strcpy(path1,path);
	char* identitas=strtok(NULL,"");
	char identitas1[strlen(identitas)];
        strcpy(identitas1,identitas);
        char* tahun=strtok(identitas,",");
        tahun=strtok(NULL,",");
	char tahun1[strlen(tahun)];
        strcpy(tahun1,tahun);

	char* token = strtok(path1, "/");
	char* save[10];
 	 int l=0;
	FILE *h=fopen("/home/fiansyah/praktikum3/finak/client/data.txt","a+");
    while (token != NULL) {
       token=strtok(NULL, "/");
        save[l]=token;
//	printf("save[%d]=%s\n",l,save[l]);
       l++;
    }
	char* format=strtok(save[l-2],".");
	format=strtok(NULL,".");
	
	fprintf(h,"\nNama:");
	fprintf(h,save[l-2]);
        fprintf(h,".");
        fprintf(h,format);
	fprintf(h,"\nPublisher:");
	fprintf(h,identitas);
	fprintf(h,"\nTahun Publishing:");
	fprintf(h,tahun1);
	fprintf(h,"\nEktensi File:");
	fprintf(h,format);
	fprintf(h,"\nFilepath:");
	fprintf(h,path);
	fprintf(h,"\n");
	fclose(h);
	
	strcpy(uname,kosong);
	char* berhasil="";
	write(sockfd, berhasil, sizeof(berhasil));
        continue;

    }
       continue;
  }
else if(flag==10){
	printf("finddddd");
        int flag1=0;
	int flag2=0;
        flag=0;
    char uname[80];
    char kosong[]=" ";
    FILE *j=fopen(filename1,"r");


    int loop=20;
    for(loop=0;loop<20;loop++)
    {
        fscanf(j,"%s",uname);
     //   printf("uname=%d\n loop=%d\n",strcmp(uname,""),loop);
        if((strcmp(uname,"")==32&&loop==0)||loginer==0){
        flag=0;
        flag1=1;
        char* berhasil="";
        write(sockfd, berhasil, sizeof(berhasil));
        break;
        }
        else ;
        if(strcmp(uname,kosong)==0)break;



	char* path=strtok(uname,",");
	FILE *h=fopen("/home/fiansyah/praktikum3/finak/client/data.txt","a+");
//	printf("strstr=%s\n",strstr(path,cek));
	if(strstr(path,cek)!=NULL){
//	printf("ketemu yesssssss\n");
		flag2=0;
		 char berhasil[]="ketemu";
                 write(sockfd, berhasil, sizeof(berhasil));

         }
        else{
         fclose(h);
        	char berhasil[]="";
     		 write(sockfd, berhasil, sizeof(berhasil));

             	continue;
         }



        char path1[strlen(path)];
        strcpy(path1,path);
        char* identitas=strtok(NULL,"");
        char identitas1[strlen(identitas)];
        strcpy(identitas1,identitas);
        char* tahun=strtok(identitas,",");
        tahun=strtok(NULL,",");
        char tahun1[strlen(tahun)];
        strcpy(tahun1,tahun);

        char* token = strtok(path1, "/");
        char* save[10];
         int l=0;
        
    while (token != NULL) {
       token=strtok(NULL, "/");
        save[l]=token;
//      printf("save[%d]=%s\n",l,save[l]);
       l++;
    }
        char* format=strtok(save[l-2],".");
        format=strtok(NULL,".");

        fprintf(h,"\nNama:");
        fprintf(h,save[l-2]);
        fprintf(h,".");
        fprintf(h,format);
        fprintf(h,"\nPublisher:");
        fprintf(h,identitas);
        fprintf(h,"\nTahun Publishing:");
        fprintf(h,tahun1);
        fprintf(h,"\nEktensi File:");
        fprintf(h,format);
        fprintf(h,"\nFilepath:");
        fprintf(h,path);
        fprintf(h,"\n");
        fclose(h);

        strcpy(uname,kosong);
        char* berhasil="";
     write(sockfd, berhasil, sizeof(berhasil));
        continue;

    }
continue;
  }




  else{
          char login[]="";
                write(sockfd, login, sizeof(login));
                continue;
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
