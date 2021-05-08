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
    // mengalikan matrix 1 & matrix 2
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
    // hasil kali matrix di buat jadi array 1D dan disimpan di value
    printf("hasil:\n");
    for(i=0; i < 4;i ++){
        for(j=0; j < 6; j++){
            printf("%d ", value[i*6 + j]);
        }printf("\n");
    }
    // soal2a berjalan trus hingga soal2b selesai
    while(1){
		if (*wait == 1){printf("soal2b.c done!\n"); break;}
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
3 2 1
1 0 2
4 0 1

mtx 2:
1 2 3 4 2 1
5 3 2 1 4 0
0 1 3 1 2 3

hasil:
11 11 16 9 16 10
13 13 16 15 16 6
1 4 9 6 6 7
4 9 15 17 10 7  
*/
