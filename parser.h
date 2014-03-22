
/*This will parse the url*/
#ifndef __PARSERH__
#define __PARSERH__

#include<stdlib.h>
#include "utils.h"

#define MAX_ARGS_SIZE 50

/*structure to split the request into path[] and args[]*/
struct URI_REQUEST {
    char *path[MAX_CHAR_SIZE];
    char *arg[MAX_CHAR_SIZE];
};


/*structure to get method, request and protocol of the request*/
struct URI {
    char request[MAX_CHAR_SIZE];
    char method[6]; //max value is 'delete'a
    char protocol[8]; //either HTTP/1.1 or HTTP/1.0

    struct URI_REQUEST req; //request path and args.
};



/*grab the first line of a string, store in "firstline"*/
int parseGetFirstLine(char *firstline, char *s) {

    int i=0;
    while(s[i] != '\n'){
        firstline[i] = s[i];
        i++;
    }
    
    firstline[i] = 0;
    return 0;
}

/*tokenize into method, request and protocol*/
struct URI parseTokenize(char *s) {
    struct URI uri;

    char x[MAX_CHAR_SIZE];
    parseGetFirstLine(x, s);

    sscanf(x, "%s %s %s", uri.method, uri.request, uri.protocol);
    return uri;
}



#endif
