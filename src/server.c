/* Moreno Garza Oscar
 * This is the web-server in an effor to port a small set of the Sinatra
 * framework for ruby into C.
 *
 * Based on http://tinyhack.com/2014/03/12/implementing-a-web-server-in-a-single-printf-call/ 
 */

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <stdbool.h>

#include "utils.h"
#include "parser.h"

static void server_loop(int socketfd);
static void server_pkg_handler(int cfd, char *packet);
static int server_init(const char *server_ip, uint16_t port);

void server_run(const char *server_ip, uint16_t port) {
    int socketfd = server_init(server_ip, port);
    server_loop(socketfd);
}

static int server_init(const char *server_ip, uint16_t port) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    int backlog = 5;
    struct sockaddr_in server_addr = { 0 };
    
    server_addr.sin_family      = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(server_ip);
    server_addr.sin_port        = htons(port);
    
    bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(sockfd, backlog);
    
    printf("C-natra has taken the stage... http://%s:%d/\n", server_ip, port);

    return sockfd;
}

/* Handle the package */
static void server_pkg_handler(int cfd, char *packet) {
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
}

/* Listen for connection and serve them*/
static void server_loop(int sockfd) {    
    while (true) {
        int cfd  = accept(sockfd, 0, 0);
        char buffer[MAX_CHAR_SIZE];
        int bufferbytes = read(cfd, buffer,  MAX_CHAR_SIZE); //get shit.

        /*fork this nigga process, if we got actual content*/
        if (fork() ==0 && bufferbytes > 0) {
            server_pkg_handler(cfd, buffer); //handle shit.
            shutdown(cfd, SHUT_RDWR); //close connection.
            close(cfd); //ditto
        }
    }  
}



