#ifndef __UTILSH__
#define __UTILSH__

#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<unistd.h>

#define MAX_CHAR_SIZE 1024
#define MAX_PACKET_SIZE 512

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

#endif
