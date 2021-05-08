#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

int main(){

    key_t key = 1234;
    int *value, *wait=0;

    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    value = shmat(shmid, NULL, 0);
    wait= shmat(shmid, NULL, 0);

    int i, j, k,l=0,temp=0, mtx1[4][6], mtx2[3][6];
    //input
    printf("matrix 1:\n");
    for(i=0; i < 4; i++){
        for(j=0; j < 3; j++) scanf("%d", &mtx1[i][j]);
    }
    printf("matrix 2:\n");
    for(i=0; i < 3; i++){
        for(j=0; j < 6; j++) scanf("%d", &mtx2[i][j]);
    } printf("\n");
    // mengalikan
    for(i=0; i < 4;i ++){
        for(j=0; j < 6; j++){
            for (k = 0; k < 3; k++){
                temp += mtx1[i][k] * mtx2[k][j];
				}
            value[i*6 + j] = temp;
            temp=0;
        }
    } 

    // cek mtx1, mtx2, hasil
    // for(i=0; i < 4; i++){
    //     for(j=0; j < 3; j++){
    //         printf("%d ", mtx1[i][j]);
    //     }printf("\n");
    // }printf("\n");
    // for(i=0; i < 3; i++){
    //     for(j=0; j < 6; j++) {
    //         printf("%d ", mtx2[i][j]);
    //     }printf("\n");
    // }printf("\n");
    for(i=0; i < 4;i ++){
        for(j=0; j < 6; j++){
            printf("%d ", value[i*6 + j]);
        }printf("\n");
    }
    while(1){
		if (*wait == 1) {printf("Lanjot"); break;}
		printf("waiting...\n"); sleep(3);
	}

    shmdt(value);
    shmdt(wait);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}
/*
mtx 1:
1 2 3
4 5 6
7 8 9
1 2 3

mtx 2:
1 2 3 4 5 6
7 8 9 10 11 12
1 2 3 4 5 6

hasil:
18 24 30 36 42 48 
45 60 75 90 105 120 
72 96 120 144 168 192 
18 24 30 36 42 48  
*/
