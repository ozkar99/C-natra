#ifndef __SERVERH__
#define __SERVERH__
/* Moreno Garza Oscar
*  This is the web-server in an effor to port a small set of the Sinatra framework for ruby into C.
*/

/* Based on http://tinyhack.com/2014/03/12/implementing-a-web-server-in-a-single-printf-call/ */
#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<arpa/inet.h>

#include "utils.h"
#include "parser.h"

/*This function starts the server, return socket file descriptor*/
int serverStart(char *servip, int port) {
 
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    int backlog = 5;

    /* bind this mofo */
    struct sockaddr_in serv_addr;
    memset((char *)&serv_addr, '\0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(servip);
    serv_addr.sin_port = htons(port);
    bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    listen(sockfd, backlog);


    /*lame sinatra suckup*/
    char servchar[MAX_CHAR_SIZE];
    sprintf(servchar, "http://%s:%d/", servip, port);
    printf("C-natra has taken the stage...%s\n", servchar);

    return sockfd;
}

/* Handle the package */
void serverPackageHandler(int cfd, char *packet) {
    char s[MAX_CHAR_SIZE];

    fflush(stdin);
    sprintf(s, "Server: %d Got:\n%s\n", cfd, packet);
    logWrite(s);

    struct URI u = parseURI(packet);

    if ( strcmp(u.method, "GET") == 0){
        serverSendHTML(cfd, "I GOT A GET");
    } else if ( strcmp(u.method, "POST") ==0 ) {
        serverSendHTML(cfd, "I GOT A POST");
    } else {
        serverSendHTML(cfd, "<html><h1>DEFAULT SHIT</H1>");
    }

    /*parsing tests*/
    //printf("\n\n\n\nParsing Test in server.c:\n");
    //printf("Method: %s\nRequestPath: %s\nRequestArgs: %s\nProtocol: %s\n", u.method, u.req.path, u.req.arg, u.protocol);
}

/* Listen for connection and serve them*/
void serverLoop(int sockfd) {    
    while (1) {
        int cfd  = accept(sockfd, 0, 0);
        char buffer[MAX_CHAR_SIZE];
        int bufferbytes = read(cfd, buffer,  MAX_CHAR_SIZE); //get shit.

        /*fork this nigga process, if we got actual content*/
        if (fork() ==0 && bufferbytes > 0) {
            serverPackageHandler(cfd, buffer); //handle shit.
            shutdown(cfd, SHUT_RDWR); //close connection.
            close(cfd); //ditto
        }
    }  
}

/************************* main ***********************/
/* main routine */ 
int main(int argc, char *argv[]) {

    int socketfd = serverStart("127.0.0.1", 30001);
    serverLoop(socketfd);

    return 0;
}

#endif
