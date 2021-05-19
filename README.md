# soal-shift-sisop-modul-3-F04-2021

# Anggota Kelompok
- **Thomas Dwi Awaka** (05111940000021)
- **Muhammad Arifiansyah** (05111940000027)
- **Muhammad Rizqullah Akbar** (05111940000178)

# Pembahasan Soal
## Soal 1

## Soal 2
Crypto adalah teman Loba. Suatu pagi, Crypto melihat Loba yang sedang kewalahan mengerjakan tugas dari bosnya. Karena Crypto adalah orang yang sangat menyukai tantangan, dia ingin membantu Loba mengerjakan tugasnya. Detil dari tugas tersebut adalah:
### Soal 2a
Membuat program perkalian matriks (4x3 dengan 3x6) dan menampilkan hasilnya. Matriks nantinya akan berisi angka 1 hingga 20. Hasil perkalian dari kedua matrix ini akan dipakai di **soal 2b**,sehingga perlu memakai _shared memory_ agar hasil perkalian tersebut dapat digunakan pada soal 2b.
_Shared memory_ pada soal 2a dimulai dengan menciptakan _segment memory_ (`shmget()`) & meng-_attach_ variabel yang akan digunakan (`shmat()`).
```C
    key_t key = 1234;
    int *value, *wait=0;

    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    value = shmat(shmid, NULL, 0);
    wait= shmat(shmid, NULL, 0);
```
- `value` akan menyimpan nilai dari hasil perkalian dari dua matrix di _shared memory_.
- `wait` akan menjadi indikator bahwa __soal 2b__ telah selesai menggunakan `value`. `wait` bernilai awal 0

Kemudian dilakukan proses memasukan nilai dari matriks 4x3 & 3x6 seperti berikut
```C
    printf("matrix 1:\n");
    for(i=0; i < 4; i++){
        for(j=0; j < 3; j++) scanf("%d", &mtx1[i][j]);
    }
    printf("matrix 2:\n");
    for(i=0; i < 3; i++){
        for(j=0; j < 6; j++) scanf("%d", &mtx2[i][j]);
    } printf("\n");
```
Setelah itu mengalikan kedua matrix tersebut dan disimpan di `value` dalam bentuk _array 1D_. Dan untuk menyesuaikan posisi setiap nilai dari matrix 2D ke matrix 1D dapat memakai rumusan `loop_baris x total_kolom + loop_kolom`.
```C
    for(i=0; i < 4;i ++){
        for(j=0; j < 6; j++){
            for (k = 0; k < 3; k++){ // mengalikan kedua matrix
                temp += mtx1[i][k] * mtx2[k][j]; 
				}
            value[i*6 + j] = temp; //memasukan nilai ke value
            temp=0;
        }
    } 
```
Hasil dari perkalian kedua matriks dapat diliat dengan looping sebagai berikut
```C
    printf("hasil:\n");
        for(i=0; i < 4;i ++){
            for(j=0; j < 6; j++){
                printf("%d ", value[i*6 + j]);
        }printf("\n");
    }
```
Agar program ```soal2a.c``` dapat tetap aktif, dilakukan looping `while` dan `sleep()` hingga nilai dari `wait` bernilai 1.
```C
    while(1){
		if (*wait == 1){printf("soal2b.c done!\n"); break;}
		printf("waiting...\n"); sleep(3);
	}
```
jika soal 2b telah selesai berjalan. `value` dan `wait` dapat dilepas/_detach_ dari _shared memory_ yang tersedia dengan cara
```C
    shmdt(value);
    shmdt(wait);
```
dan dilakukan proses control terhadap _shared memory_ dengan `smctl`. Proses yang dilakukan adalah `IPC_RMID`. Proses ini menghapus/_remove_ _shared memory_ berdasarkan `smid` yang tadi diperoleh pada saat menciptakan _segment memory_.
```C
shmctl(shmid, IPC_RMID, NULL);
```
### Soal 2b
Membuat program dengan menggunakan matriks output dariprogram soal2a.c. Kemudian matriks tersebut akan dilakukan perhitungan dengan matrix baru (input user) sebagai berikut contoh perhitungan untuk matriks yang ada. Perhitungannya adalah setiap cel yang berasal dari matriks A menjadi angka untuk faktorial, lalu cel dari matriks B menjadi batas maksimal faktorialnya. Perhitungan setiap cel menggunakan `thread`.

Sama seperti soal 2a, terlebih dahulu menciptakan _segment memory_ (`shmget()`) & meng-_attach_ variabel yang akan digunakan (`shmat()`). `value` akan menyimpan nilai dari hasil perkalian dari dua matrix di _shared memory_. `wait` akan menjadi indikator bahwa __soal 2b__ telah selesai menggunakan `value`. `wait` bernilai awal 0.
Setelah menciptakan _segment memory_, program dapat menerima input untuk matriks 4x6 yang baru dalam bentuk _array 1D_, karena menyesuaikan dengan `value` yang berbentuk _array 1D_.
```C
    printf("mtx3:\n");
    for(i = 0; i < 24; i++) scanf("%d", &mtx3[i]);
```
Kemudian matriks baru tersebut menjalankan perhitingan faktorial dengan menggunakan thread. Dimulai dengan membuat thread/`pthread_create()` terlebih dahulu dengan:
- `tid1[]` digunakan untuk menunjukkan alamat memori dengan thread ID dari thread baru & inisialisasi array untuk menampung thread
- nilai attr di-set __NULL__ agar thread menggunakan atribut default.
- Thread yang baru dibuat akan berjalan dimulai dari fungsi factorial() dalam fungsi thread.
- Pointer `address` digunakan untuk memberikan sebuah argumen ke fungsi factorial()

Dan dengan `pthread_join(tid1[address], NULL)` untuk memastikan bahwa thread `tid1[address]` berhasil berhenti dan mengembalikan nilai NULL jika berhasil berhenti.
```C
    #include <pthread.h>
    pthread_t tid1[24];
...
    for(i = 0; i < 4; i++){
        for(j = 0; j < 6; j++){
            address = i*6+j; //posisi array 1D
            pthread_create(&(tid1[address]), NULL, 
            &factorial, (void *) &address);
            pthread_join(tid1[address], NULL);
        }
    }
```
Untuk proses perhitungan faktorial dari kedua matriks ini perlu memperhatikan beberapa syarat:
- jika nilai cel matriks `value` >= matriks baru, maka hasil faktorial = value!/(value-baru)!
- jika nilai cel matriks baru > matriks `value` , maka hasil faktorial = value!
- jika nilai cel matriks `value` = 0, maka hasil faktorial = 0

`*arg` yang di-_passing_ dari `pthread_create()` berisi posisi dari cel yang akan dihitung, yaitu `i*6+j` dan hasil faktorial disimpan di array 1D `ans[]`
```C
    void* factorial(void *arg){
        int *addressTemp = (int*) arg;
        if(value[*addressTemp] >= mtx3[*addressTemp]){
            ans[*addressTemp] +=             factcal(value[*addressTemp])/factcal(value[*addressTemp]-mtx3[*addressTemp]);
        }
        else if (value[*addressTemp] < mtx3[*addressTemp]){
            ans[*addressTemp] += factcal(value[*addressTemp]);
        } else ans[*addressTemp] = 0;
    }
```
terdapat fungsi rekursif `factcal()` untung menghitunga faktorial dari nilai yang dipakai
```C
    unsigned long factcal(int n){
        return (n>=1) ? n*factcal(n-1) : 1;
    }
```
Setelah menyelesaikan perhitungan faktorial, maka hasil tersebut dapat dicetak.
```C
    printf("\nhasil fact:\n");
    for(int i = 0; i < 4  ; i++){
    	for(int j = 0; j < 6; j++){
	    	printf("%lu ", ans[i*6+j]);
	    }
    	printf("\n");
    }
```
Ketika soal2b.c telah selesai menggunakan nilai dari `value`, maka nilai `*wait` dirubah menjadi 1 agar menginfokan soal2a.c bahwa soal2b.c telah selesai. `value` dan `wait` dapat dilepas/detach dari shared memory yang tersedia dengan cara
```C
    *wait = 1;
    shmdt(value);
	shmdt(wait);
    shmctl(shmid, IPC_RMID, NULL);
```
### Soal 2c
Karena takut _lag_ dalam pengerjaannya membantu Loba, Crypto juga membuat program __IPC Pipes__ untuk mengecek 5 proses teratas apa saja yang memakan resource komputernya dengan command `ps -aux | sort -nrk 3,3 | head -5`

### Output
#### Soal 2a
Proses memasukankan nilai setiap matriks
Hasil perkalian matriks dan _looping_ yang berjalan hingga soal 2b selesai
#### Soal 2b
Proses memasukankan nilai setiap matriks yang baru
Hasil perhitungan faktorial setiap cel pada matriks
#### Soal 2c
Output dari `soal2c.c`
### Kendala
Sempat kebingungan untuk menentukan pipe yang di-`close()` dan penggunaan `dup2()`

## Soal 3
