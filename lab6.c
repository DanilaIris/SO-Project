#include<stdio.h>
#include<stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

// Functie pentru a crea snapshot-ul directorului
void createSnapshot(const char *dirPath) {
    int snapshotFile = open("snapshot.txt", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
    if (snapshotFile < 0) {
        perror("Eroare la crearea snapshot-ului");
        exit(EXIT_FAILURE);
    }

    DIR *dir = opendir(dirPath);
    if (dir == NULL) {
        perror("Eroare la deschiderea directorului");
        exit(EXIT_FAILURE);
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        char filePath[1024];
        snprintf(filePath, sizeof(filePath), "%s/%s", dirPath, entry->d_name);

        struct stat fileStat;
        if (stat(filePath, &fileStat) < 0) {
            perror("Eroare la obtinerea informatiilor despre fisier");
            continue;
        }

        char buffer[1024];
        int length = snprintf(buffer, sizeof(buffer), "%s\t%ld\n", entry->d_name, (long)fileStat.st_mtime);
        write(snapshotFile, buffer, length);
    }

    closedir(dir);
    close(snapshotFile);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Utilizare: %s \n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *dirPath = argv[1];
    createSnapshot(dirPath);
    printf("Snapshot-ul directorului \"%s\" a fost creat cu succes.\n", dirPath);

    return 0;
}