#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "utils.h"

/*dem log writting function*/
int logWrite(char *s) {

    /*open the log get on the floor...*/
    FILE *logfd = fopen("log/info.txt", "a+");

    if (logfd == NULL) {
        perror("cant open log!");
    } else {
        /*everybody writes the dinosaur~*/
        printf("%s\n", s);
        fprintf(logfd, "%s\n", s);
        fclose(logfd);
    }

    return 0;
}

/*check if the given file exists*/
int fileExists(char *s) {

    FILE *file = fopen(s, "r");
    if (file == NULL) {
        return 0;
    } else {
        fclose(file);
        return 1;
    }
}

/* return contents of a file*/
char *getFile(char *s) {

    FILE *fp = fopen(s, "r");

    
    if ( fp != NULL) {    


        fseek(fp, 0, SEEK_END);
        long length = ftell(fp);

        fseek(fp, 0, SEEK_SET);
    
        char* buffer = malloc((length + 1) * sizeof(char));    
        long offset = 0;

        char c;

        while ( (c = fgetc(fp)) != EOF) {
                buffer[offset++] = c;
        }

        buffer[offset] = '\0';
        fclose(fp);

        return buffer;
    }
    
    return "";
    
}

/*This function sends html to the sockfd server*/
int serverSendHTML(int cfd, char *html) {
    char *ok = "HTTP/1.0 200\r\nContent-type:text/html\r\n\r\n";

    char resp[MAX_CHAR_SIZE];
    strncpy(resp, ok, MAX_CHAR_SIZE);
    strncat(resp, html, MAX_CHAR_SIZE);

    /*send to the server*/
    write(cfd, resp, strlen(resp));
    return 0;
}
