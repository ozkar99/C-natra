
/*This will parse the url*/
#ifndef __PARSERH__
#define __PARSERH__

#include<stdlib.h>
#include "utils.h"
#define MAX_ARGS_SIZE 50


struct URI {
    char *args[MAX_ARGS_SIZE];
    char *path[MAX_ARGS_SIZE];
    char method[6]; //max value is 'delete'
};


/*grab the first line, of the header*/

int parseGetFirstLine(char *firstline, char *s) {

    int i=0;
    while(s[i] != '\n'){
        firstline[i] = s[i];
        i++;
    }
    
    firstline[i] = 0;
    return 0;
}

#endif
