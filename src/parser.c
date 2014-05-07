#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utils.h"
#include "parser.h"

#define DEBUG_FLAG 1

/*return extension of a file*/
char *parseGetExtension(struct URI u) {

    char filename[MAX_CHAR_SIZE];
    strncpy(filename, u.req.path[0], MAX_CHAR_SIZE);    

    char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return "";
    
    char *ext = malloc(MAX_EXT_SIZE); //allocate memory so the variable doesnt get destroyed later on.
    strncpy(ext, dot+1, strlen(dot+1) - 1); //remove the last char which is a '/' due to the sanitizer being a crackass.

    return ext;
}


/*check if file is a CHTML file*/
int parseCheckCHTML(struct URI u) {
    
    if ( (strncmp(parseGetExtension(u), "chtml", 5) == 0)
      || (strncmp(parseGetExtension(u), "CHTML", 5) == 0) ) {
        return 1;
    }
    
    return 0;
}


/*grab the first line of a string, store in "firstline"*/
int parseGetFirstLine(char *firstline, char *s) {
    int i = 0;
    while(s[i] != '\n'){
        firstline[i] = s[i];
        i++;
    }

    if (i > MAX_CHAR_SIZE) {
        i = MAX_CHAR_SIZE;
    }
    
    firstline[i] = '\0';
    return 1;
}

/* check if the request, contains arguments (look for a given character)*/
int parseContainsChar(char *s, char c) {
    int i = 0;

    for(i = 0; i <= strlen(s); i++) {
        if (s[i] == c) 
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
        strncpy(uri->method, method, MAX_CHAR_SIZE);
        strncpy(uri->request, request, MAX_CHAR_SIZE);
        strncpy(uri->protocol, protocol, MAX_CHAR_SIZE);
        return 1;
    } else {
        return 0;
    }
}

/*split the request*/
int parseSplitRequest(struct URI *u) {
    char *separator = "?";

    /* first split the request via args and path */
    if ( parseContainsChar(u->request, '?') ) {
        /*split the ? first if we got them*/
        char *path = strtok(u->request, separator);
        char *args = strtok(NULL, separator);
        strncpy(u->req.path[0], path, MAX_CHAR_SIZE);
       
        if (args == NULL) {
            strncpy(u->req.arg[0], "null", MAX_CHAR_SIZE);
        } else { 
            strncpy(u->req.arg[0], args, MAX_CHAR_SIZE);
        }

    } else {
        /*default values*/
        strncpy(u->req.path[0], u->request, MAX_CHAR_SIZE);
        strncpy(u->req.arg[0], "null\0", MAX_CHAR_SIZE);
    }

    return 1;
}

/*sanitize the path and arguments*/
int parseSanitizeUri(struct URI *u) {
    int lastPath = strlen(u->req.path[0]) - 1;
    int lastArg = strlen(u->req.arg[0]) - 1;

    /*Insert always the trailing slash*/
    if (u->req.path[0][lastPath] != '/' && strncmp(u->req.path[0], "/", 2) != 0) { 
        u->req.path[0][lastPath+1] = '/';
    }

    /*Remove the trailing &*/
    if ( strncmp(u->req.arg[0], "null", 4) != 0 && u->req.arg[0][lastArg] == '&') {                
        u->req.arg[0][lastArg] = '\0'; 
        strncpy(u->req.val[0], u->req.arg[0], MAX_CHAR_SIZE); //copy into the value. 
    }

    return 1;
}

/*parse the path and arguments according*/
int parseRequest(struct URI *u) {
    char *pathToken = "/";
    char *argToken = "&";

    /* PATH PART*/
    //make copy so we leave u->req.path[0] and u->req.arg[0]/u->req.val[0] alone.
    char reqPath[MAX_CHAR_SIZE];
    strncpy(reqPath, u->req.path[0], MAX_CHAR_SIZE);

    //first token for '/'    
    char *currp = strtok(reqPath, pathToken);

    int i=1; //starting position for URI.args
    while (currp) {
        strncpy(u->req.path[i], currp, MAX_CHAR_SIZE);  //copy the current token at each '/' interval.
        i++; //increment the path.
        currp = strtok(NULL, pathToken); //get next token.
    }

    /* ARGS PART */
    char argsPath[MAX_CHAR_SIZE];
    strncpy(argsPath, u->req.arg[0], MAX_CHAR_SIZE);

    char *currargs = strtok(argsPath, argToken);
    int j = 0;
    char arguments[MAX_ARGS_SIZE][MAX_CHAR_SIZE];

    //if we have more than 1 arg keep tokenizing.
    if (parseContainsChar(u->req.arg[0], '&') ) {
        while(currargs) {
            strcpy(arguments[j], currargs);
            /*Advance pointer*/
            currargs = strtok(NULL, argToken);
            j++;        
        }
    }

    int k=0;
    for(k=0 ;  k < j; k++) {
        /* parse each element in the array 
         * separe left and right 
         */
        char *left, *right;

        if ( parseContainsChar(arguments[k], '=') ) {
            left = strtok(arguments[k], "="); 
            right = strtok(NULL, "=");
        } else {
            left = arguments[k];
            right = ""; //harcoded true...
        }

        printf("%s = %s\n", left, right);

        strcpy(u->req.arg[k+1], left);  //K+1, since arg[0] contains the whole argument string
        strcpy(u->req.val[k+1], right);
    }

    return 1;
}


/* parse the uri, main entry point */
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
    if ( DEBUG_FLAG == 1) {
        printf("\n\nparser.h\n");
        printf("Extension: %s\n", parseGetExtension(uri));
    }

    return uri;

}
