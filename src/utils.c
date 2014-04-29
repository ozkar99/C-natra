#include <stdlib.h>
#include <stdio.h>
#include "utils.h"

#define MESSAGE_FORMAT "%s\n"

void log_write(const char *message) {
    FILE *logfd = fopen("log/info.txt", "a+");

    if(logfd != NULL) {
        printf(MESSAGE_FORMAT, message);
        fprintf(logfd, MESSAGE_FORMAT, message);
        fclose(logfd);
    }
    else {
        perror("Can't open log file.");
    }
}
