//P429
#include "error_functions.h"

#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    char template[] = "/tmp/testXXXXX";
    int fd = mkstemp(template);
    if (fd == -1) {
        errExit("mkstemp");
    }
    printf("file offset before fork(): %lld\n", (long long) lseek(fd, 0, SEEK_CUR));

    int flags;
    flags = fcntl(fd, F_GETFL);
    if (flags == -1) {
        errExit("fcntl");
    }
    printf("O_APPEND flag before fork() is: %s\n", (flags & O_APPEND) ? "on" : "off");

    switch (fork()) {
        case -1:
            errExit("fork");
        case 0:
            if (lseek(fd, 1000, SEEK_SET) == -1) {
                errExit("lseek");
            }
            flags = fcntl(fd, F_GETFL);
            if (flags == -1) {
                errExit("fcntl");
            }
            flags |= O_APPEND;
            if (fcntl(fd, F_SETFL, flags) == -1) {
                errExit("fcntl");
            }
            exit(0);
        default:
            if (wait(NULL) == -1) {
                errExit("wait");
            }
            printf("child has exited\n");

            printf("file offset in parent: %lld\n", (long long) lseek(fd, 0, SEEK_CUR));
            flags = fcntl(fd, F_GETFL);
            if (flags==-1){
                errExit("fcntl");
            }
            printf("O_APPEND flag in parent is: %s\n", (flags & O_APPEND)?"on":"off");
            exit(0);
    }
}