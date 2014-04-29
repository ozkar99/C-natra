/* Moreno Garza Oscar
 * This is the web-server in an effort to port a small set of the Sinatra
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
#include "server.h"

static int init(const char *server_ip, uint16_t port);
static void loop(int socketfd);
static void pkg_handler(int cfd, char *packet);
static void send_html(int cfd, const char *html);

void server_run(const char *server_ip, uint16_t port) {
    int socketfd = init(server_ip, port);
    loop(socketfd);
}

static int init(const char *server_ip, uint16_t port) {
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

static void loop(int sockfd) {   
    while (true) {
        int cfd = accept(sockfd, 0, 0);
        char buffer[MAX_CHAR_SIZE];
        int bufferbytes = read(cfd, buffer, MAX_CHAR_SIZE);

        if (fork() == 0 && bufferbytes > 0) {
            pkg_handler(cfd, buffer);
            shutdown(cfd, SHUT_RDWR);
            close(cfd);
        }
    }
}

static void pkg_handler(int cfd, char *packet) {
    char s[MAX_CHAR_SIZE];

    fflush(stdin);
    sprintf(s, "Server: %d Got:\n%s\n", cfd, packet);
    log_write(s);

    struct URI u = parseURI(packet);

    if ( strcmp(u.method, "GET") == 0){
        send_html(cfd, "I GOT A GET");
    } else if ( strcmp(u.method, "POST") ==0 ) {
        send_html(cfd, "I GOT A POST");
    } else {
        send_html(cfd, "<html><h1>DEFAULT SHIT</H1>");
    }
}

static void send_html(int cfd, const char *html) {
    char *http_code = "HTTP/1.0 200\r\nContent-type:text/html\r\n\r\n";
    char resp[MAX_CHAR_SIZE];
    
    strncpy(resp, http_code, MAX_CHAR_SIZE);
    strncat(resp, html, MAX_CHAR_SIZE);
    
    write(cfd, resp, strlen(resp));
}
