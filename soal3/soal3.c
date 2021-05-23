#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>

//pthread_mutex_t signal;

char *getName(char *dir)
{
    char *name = strrchr(dir, '/');
    if (name == dir)
        return "";
    return name + 1;
}

char *toLower(char *str)
{
    unsigned char *temp = (unsigned char *)str;
    while (*temp)
    {
        *temp = tolower(*temp);
        temp++;
    }
    return str;
}

char *getExt(char *dir)
{
    char *unk = {"Unknown"};
    char *hid = {"Hidden"};
    char *tmp = strrchr(dir, '/');

    if (tmp[1] == '.')
        return hid;

    int i = 0;
    while (i < strlen(tmp) && tmp[i] != '.')
        i++;
    if (i == strlen(tmp))
        return unk;

    char ext[400];
    int j = i;
    while (i < strlen(tmp))
        ext[i - j] = tmp[i], i++;
    return toLower(ext + 1);
}

void *categorizeFile(void *arg)
{
    char *src = (char *)arg;
    char srcP[150];
    memcpy(srcP, (char *)arg, 400);
    char *srcExt = getExt(src);
    char ext[400];
    strcpy(ext, srcExt);

    DIR *dir = opendir(srcExt);
    if (dir)
        closedir(dir);
    else if (ENOENT == errno)
        mkdir(srcExt, 0666);

    char *srcName = getName(srcP);
    char *cwd = getenv("PWD");

    char destP[10000];
    sprintf(destP, "%s/%s/%s", cwd, ext, srcName);
    rename(srcP, destP);
}

void categorizeDirectory(char *folderPath, int threadSize)
{
    DIR *directory = opendir(folderPath);
    struct dirent *dr;
    pthread_t tid[threadSize];
    int count = 0;
    char fileName[400][400];

    while ((dr = readdir(directory)) != NULL)
    {
        if (dr->d_type == DT_REG)
        {
            sprintf(fileName[count], "%s/%s", folderPath, dr->d_name);
            pthread_create(&tid[count], NULL, categorizeFile, (void *)fileName[count]);
            count++;
        }
        else if ((dr->d_type == DT_DIR) && strcmp(dr->d_name, ".") != 0 && strcmp(dr->d_name, "..") != 0)
        {
            char folderPath2[400];
            sprintf(folderPath2, "%s/%s", folderPath, dr->d_name);
            DIR *directory2 = opendir(folderPath2);
            struct dirent *dr2;
            int threadSize2 = 0;
            while ((dr2 = readdir(directory2)) != NULL)
            {
                if (dr2->d_type == DT_REG)
                {
                    threadSize2++;
                }
            }
            categorizeDirectory(folderPath2, threadSize2);
            closedir(directory2);
        }
    }

    for (int i = 0; i < threadSize; i++)
        pthread_join(tid[i], NULL);
    closedir(directory);
}

int main(int argc, char *argv[])
{
    if (argc < 2)
        exit(EXIT_FAILURE);

    else if (argc > 2 && strcmp(argv[1], "-f") == 0)
    {
        int length = argc - 2;
        pthread_t tid[length];
        int count = 0;
        for (int i = 2; i < argc; i++)
        {
            if (access(argv[i], F_OK) == 0)
            {
                pthread_create(&tid[count], NULL, categorizeFile, (void *)argv[i]);
                count++;
                printf("File %d : Berhasil dikategorikan\n", i - 1);
            }
            else
            {
                printf("File %d : Sad, gagal :(\n", i - 1);
            }
        }
        for (int i = 0; i < count; i++)
            pthread_join(tid[i], NULL);
        return 0;
    }

    else if (argc == 3 && strcmp(argv[1], "-d") == 0)
    {
        DIR *directory = opendir(argv[2]);
        if (directory)
        {
            struct dirent *dr;
            int thread = 0;
            while ((dr = readdir(directory)) != NULL)
            {
                if (dr->d_type == DT_REG)
                {
                    thread++;
                }
            }
            categorizeDirectory(argv[2], thread);
            closedir(directory);
            printf("Direktori Sukses disimpan!\n");
        }
        else if (ENOENT == errno)
            printf("Yah, gagal disimpan :(\n");
    }

    else if (argc == 2 && strcmp(argv[1], "*") == 0)
    {
        char *cwd = getenv("PWD");
        DIR *directory = opendir(cwd);

        struct dirent *dr;
        int thread = 0;
        while ((dr = readdir(directory)) != NULL)
        {
            if (dr->d_type == DT_REG)
            {
                thread++;
            }
        }
        categorizeDirectory(cwd, thread);
        closedir(directory);
    }

    else{
        printf("Argumen salah\n");
        return 0;
    }
}
