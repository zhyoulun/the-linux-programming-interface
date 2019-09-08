#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

static void outputError(int useError, int err, int flushStdout, const char *format, va_list ap) {
    const int SIZE = 500;
    char buf[SIZE], userMsg[SIZE], errText[SIZE];
    vsnprintf(userMsg, SIZE, format, ap);
    if (useError) {
        snprintf(errText, SIZE, " [%d %s]", err, strerror(err));
    } else {
        snprintf(errText, SIZE, ":");
    }
    snprintf(buf, SIZE, "ERROR%s %s\n", errText, userMsg);
    if (flushStdout) {
        fflush(stdout);
    }
    fputs(buf, stderr);
    fflush(stderr);
}

void errExit(const char *format, ...) {
    va_list argList;
    va_start(argList, format);
    outputError(1, errno, 1, format, argList);
    va_end(argList);

    exit(1);
}

void fatal(const char *format, ...) {
    va_list argList;
    va_start(argList, format);
    outputError(0, 0, 1, format, argList);
    va_end(argList);

    exit(1);
}