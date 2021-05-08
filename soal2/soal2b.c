#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>
#include <unistd.h>

pthread_t tid1[24];
int *value, *wait, valTemp[24], addressTemp, address, i, j, mtx3[24];
unsigned long ans[24];
unsigned long factcal(int n) {
   return (n>=1) ? n*factcal(n-1) : 1;
}

void* factorial(void *arg){
    int *addressTemp = (int*) arg;
    // printf("addressTemp: %d\n", *addressTemp);
    // printf("a: %d | b: %d\n", value[*addressTemp], mtx3[*addressTemp]);
    if(value[*addressTemp] >= mtx3[*addressTemp]){
        ans[*addressTemp] += factcal(value[*addressTemp])/factcal(value[*addressTemp]-mtx3[*addressTemp]);
    }
    else if (value[*addressTemp] < mtx3[*addressTemp]){
        ans[*addressTemp] += factcal(value[*addressTemp]);
    } else ans[*addressTemp] = 0;
    // printf("final: %d\n", ans[*addressTemp]);
}

int main(){
    key_t key = 1234;

    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    value = shmat(shmid, NULL, 0);
    wait= shmat(shmid, NULL, 0);

    // input matrix baru
    printf("mtx3:\n");
    for(i = 0; i < 24; i++) scanf("%d", &mtx3[i]);

    //thread
    for(i = 0; i < 4; i++){
        for(j = 0; j < 6; j++){
            address = i*6+j;
            pthread_create(&(tid1[address]), NULL, 
            &factorial, (void *) &address);
            pthread_join(tid1[address], NULL);
            
        }
    }
    printf("\nmtx dari 2a:\n");
    for(int i = 0; i < 4  ; i++){
		for(int j = 0; j < 6; j++){
			printf("%d ", value[i*6+j]);
		}
		printf("\n");
	}
    printf("\nhasil fact:\n");
    for(int i = 0; i < 4  ; i++){
		for(int j = 0; j < 6; j++){
			printf("%lu ", ans[i*6+j]);
		}
		printf("\n");
	}

    *wait = 1;
    shmdt(value);
	shmdt(wait);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}
/*
mtx3:
1 2 3 4 2 1
5 3 2 1 4 1
2 1 3 1 2 3
1 2 3 4 3 2

value:
11 11 16 9 16 10
13 13 16 15 16 6
1 4 9 6 6 7
4 9 15 17 10 7 

hasil faktorial:
11 110 3360 3024 10
154440 1716 240 15 43680 6
1 4 504 6 30 210
4 72 2730 57120 720 42

*/
