#ifndef C_STUDY_ERROR_FUNCTIONS_H
#define C_STUDY_ERROR_FUNCTIONS_H

static void outputError(int useError, int err, int flushStdout, const char *format, va_list ap);

void errExit(const char *format, ...);

void fatal(const char *format, ...);

#endif //C_STUDY_ERROR_FUNCTIONS_H
