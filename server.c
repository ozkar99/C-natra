/* Moreno Garza Oscar
*  This is the web-server in an effor to port a small set of the Sinatra framework for ruby into C.
*/

/* Based on http://tinyhack.com/2014/03/12/implementing-a-web-server-in-a-single-printf-call/ */
#include<stdio.h>
#include<string.h>
#include<arpa/inet.h>

#include "utils.h"

/*This function starts the server, return socket file descriptor*/
int serverStart(char *servip, int port) {
 
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    int backlog = 5;

    /* bind this mofo */
    struct sockaddr_in serv_addr;
    bzero((char *)&serv_addr, sizeof(serv_addr));
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
    serverSendHTML(cfd, "<html><h1>DEFAULT SHIT</H1> \
                            <body> \
                                <form name=\"nameinput\" method=\"post\"> \
                                    USER:<input type=\"text\" name=\"user\" > \
                                    ID:<input type=\"text\" name=\"id\" > \
                                    <input type=\"submit\" value=\"Submit\" > \
                                </form> \
                            </body> \
                        </HTML>");
    sprintf(s, "Server: %d Got:\n%s\n", cfd, packet);
    logWrite(s);
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
