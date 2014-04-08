/*This will parse the url*/
#ifndef __PARSERH__
#define __PARSERH__

#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "utils.h"

#define MAX_ARGS_SIZE 50
#define MAX_PATH_SIZE 50

/* structure to split the request into path[] and args[]
 * path[0] contains the unsplitted path
 * args[0] contains the unsplitted arguments, "null" if there are no arguments
 * 
 * args[] and val[] are inyective, this means, val[n] will contain the value for key arg[n]
 * i am too lazy to implement a proper hash, might do some time later.
 */

struct URI_REQUEST {
    char path[MAX_PATH_SIZE][MAX_CHAR_SIZE];
    char arg[MAX_ARGS_SIZE][MAX_CHAR_SIZE]; 
    char val[MAX_ARGS_SIZE][MAX_CHAR_SIZE]; 
};


/*structure to get method, request and protocol of the request*/
struct URI {
    char request[MAX_CHAR_SIZE];//request
    char method[MAX_CHAR_SIZE]; //max value lenght is 'delete'
    char protocol[MAX_CHAR_SIZE]; //either HTTP/1.1 or HTTP/1.0
    char postVals[MAX_CHAR_SIZE]; //value to store the post arguments.
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
    return 1;
}

/* check if the request, contains arguments (look for ? symbol)*/
int parseContainsArgs(char *s) {
    int i=0;
    for(i=0; i<= strlen(s); i++) {
        if (s[i] == '?') 
            return 1;
    }
    return 0;
}


/* spit method, request, protocol*/
int parseSplitMRP(struct URI *uri,  char *s) {

    char *c = " \t";
    
    char *method = strtok(s, c); //only send string on first call
    char *request = strtok(NULL, c);
    char *protocol = strtok(NULL, c);

    if (method != NULL && 
        request != NULL && 
        protocol != NULL)  
    {
        strcpy(uri->method, method);
        strcpy(uri->request, request);
        strcpy(uri->protocol, protocol);
        return 1;
    } else {
        return 0;
    }
}

/*split the request*/
int parseSplitRequest(struct URI *u) {

    char *separator = "?";

    char pathStr[MAX_CHAR_SIZE];
    char argStr[MAX_CHAR_SIZE];

/* GET*/
if( strcmp(u->method, "GET") == 0 ) {
 
    if ( parseContainsArgs(u->request) ) {
        /*split the ? first if we got them*/
        char *path = strtok(u->request, separator);
        char *args = strtok(NULL, separator);
        strcpy(u->req.path[0], path);
        strcpy(u->req.arg[0], args);
    } else {
        /*default values*/
        strcpy(u->req.path[0], u->request);
        strcpy(u->req.arg[0], "null\0");
    }

/*POST*/
} else if ( strcmp(u->method, "POST") == 0) {
    /*post code here*/
} else {
    /*not post neither get code here*/
}

    return 1;

}

/*sanitize the path and arguments*/
int parseSanitizeUri(struct URI *u) {

    int lastPath = strlen(u->req.path[0]) - 1;
    int lastArg = strlen(u->req.arg[0]) - 1;

    /*Insert always the trailing slash*/
    if (u->req.path[0][lastPath] != '/' && u->req.path[0] != "/") { 
        u->req.path[0][lastPath+1] = '/';
    }

    /*Remove the trailing &*/
    if ( strcmp(u->req.arg[0], "null") != 0 && u->req.arg[0][lastArg] == '&') {                
        u->req.arg[0][lastArg] = '\0'; 
        strcpy(u->req.val[0], u->req.arg[0]); //copy into the value. 
    }


    return 1;
}

/*parse the path and arguments according*/
int parseRequest(struct URI *u) {
   

    printf("START: parseRequest\n"); 
    char *pathToken = "/";
    char *argToken = "&";

    //make copy so we leave u->req.path[0] and u->req.arg[0]/u->req.val[0] alone.
    char reqPath[MAX_CHAR_SIZE];
    strcpy(reqPath, u->req.path[0]);

    //first token for '/'    
    char *currp = strtok(reqPath, pathToken);
    printf("First Token: %s\n", currp);

    int i=1; //starting position for URI.args
    while (currp) {
           printf("\n\tCurrent Token: %s\tNumber: %d\n", currp, i);

           strcpy(u->req.path[i], currp);  //copy the rest at each / interval.
           i++; //increment the path.
           currp = strtok(NULL, pathToken); //get next token.
    
    }

    printf("END: parseRequest\n"); 
    return 1;
}


/*parse the uri, main entry point*/
struct URI parseURI(char *s) {
    struct URI uri;
    char x[MAX_CHAR_SIZE];
    int status;

    status = parseGetFirstLine(x, s);

    /*tokenize into method, request and protocol*/
    if (status == 1) 
        status = parseSplitMRP(&uri, x); 

    /*split the request in path and args*/
    if (status == 1) 
        status = parseSplitRequest(&uri);

    /*sanitize the uri*/
    if (status ==1)
        status = parseSanitizeUri(&uri);

    /*split the reqeust into path[] and arg[]*/
    if(status ==1)
        status = parseRequest(&uri);

    /*debug shit*/
    printf("\n\nparser.h\n");
    printf("Method: %s\nRequestPath: %s\nRequestArgs: %s\nProtocol: %s\n", uri.method, uri.req.path[0], uri.req.arg[0], uri.protocol);
    /*end debug shit*/

    return uri;

}
#endif