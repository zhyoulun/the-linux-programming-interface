//P734
#include "error_functions.h"

#include <unistd.h>

int main(int argc, char *argv[]) {
    int pfd[2];//pfd[1]可以写入，pfd[0]可以读取
    const int BUF_SIZE = 10;
    char buf[BUF_SIZE];

    if (argc != 2 || strcmp(argv[1], "--help") == 0) {
        printf("%s string\n", argv[0]);
        return 0;
    }

    if (pipe(pfd) == -1) {
        errExit("pipe");
    }

    ssize_t numRead;

    //父进程写数据，子进程读数据
    switch (fork()) {
        case -1:
            errExit("fork");
        case 0:
            if (close(pfd[1]) == -1) {
                errExit("close - child");
            }
            for (;;) {
                numRead = read(pfd[0], buf, BUF_SIZE);
                if (numRead == -1) {
                    errExit("read");
                }
                if (numRead == 0) {//end of file
                    break;
                }
                if (write(STDOUT_FILENO, buf, numRead) != numRead) {
                    fatal("child - partial / failed write");
                }
            }
            write(STDOUT_FILENO, "\n", 1);
            if (close(pfd[0]) == -1) {
                errExit("close");
            }
            exit(0);
        default:
            if (close(pfd[0]) == -1) {
                errExit("close - parent");
            }
            if (write(pfd[1], argv[1], strlen(argv[1])) != strlen(argv[1])) {
                fatal("parent - partial / failed write");
            }
            if (close(pfd[1]) == -1) {
                errExit("close");
            }
            wait(NULL);
            exit(0);
    }
}