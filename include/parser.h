/*This will parse the url*/
#ifndef PARSER_H
#define PARSER_H

#define MAX_ARGS_SIZE 50
#define MAX_PATH_SIZE 50
#define MAX_CHAR_SIZE 1024

/* 
 * structure to split the request into path[] and args[]
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
int parseGetFirstLine(char *firstline, char *s);

/* check if the request, contains arguments (look for a given character)*/
int parseContainsChar(char *s, char c);

/* spit method, request, protocol*/
int parseSplitMRP(struct URI *uri,  char *s);

/*split the request*/
int parseSplitRequest(struct URI *u);

/*sanitize the path and arguments*/
int parseSanitizeUri(struct URI *u);

/*parse the path and arguments according*/
int parseRequest(struct URI *u);

struct URI parseURI(char *s);

#endif
