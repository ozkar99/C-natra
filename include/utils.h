#ifndef UTILS_H
#define UTILS_H

#define MAX_CHAR_SIZE 1024
#define MAX_PACKET_SIZE 512

int logWrite(char *s);
int serverSendHTML(int cfd, char *html);
char *getFile(char *s);
int fileExists(char *s);

#endif
