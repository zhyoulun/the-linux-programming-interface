//P57
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include "error_functions.h"

int main(int argc, char *argv[]) {
    int inputFd, outputFd;

    const int SIZE = 1024;
    char buf[SIZE];

    if (argc != 3 || strcmp(argv[1], "--help") == 0) {
        printf("usage: %s old-file new-file\n", argv[0]);
        return 0;
    }

    inputFd = open(argv[1], O_RDONLY);
    if (inputFd == -1) {
        errExit("open file %s", argv[1]);
    }

    int openFlags = O_CREAT | O_WRONLY | O_TRUNC;
    int filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
    outputFd = open(argv[2], openFlags, filePerms);
    if (outputFd == -1) {
        errExit("open file %s", argv[2]);
    }

    int numRead;
    while ((numRead = read(inputFd, buf, SIZE)) > 0) {
        if (write(outputFd, buf, numRead) != numRead) {
            fatal("couldn't write whole buffer");
        }
    }
    if (numRead == -1) {
        errExit("read");
    }

    if (close(inputFd) == -1) {
        errExit("close input");
    }
    if (close(outputFd) == -1) {
        errExit("close output");
    }

    return 0;
}