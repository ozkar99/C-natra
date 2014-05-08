#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "parser.h"
#include "handler.h"
#include "utils.h"

/*This should return the html page, when given an uri as parameter*/
char *handlerUri(struct URI u) {
    if ( strncmp(u.method, "GET", 3) == 0 ) {
        return handlerGET(u);
    } else if ( strncmp(u.method, "POST", 4) == 0 ) {
        return handlerPOST(u);
    } else {
        return "I GOT SOMETHING ELSE";
    }
}



/* Method handling */
char *handlerGET(struct URI u) {


    char filename[MAX_CHAR_SIZE];
    snprintf(filename, sizeof(filename), "%s%s", "app/public", u.req.path[0]);
    filename[strlen(filename)-1] = '\0'; //get rid of trailing slash

    if (fileExists(filename) && !(strcmp(filename, "app/public") == 0) ){

        if ( parseCheckCHTML(u) ) {
            /*implement templating...*/
            return "NO TEMPLATES YET";
        } else {
            /*grab the file, read it, return it as string.
            this works great for html or txt files, but thats about it...*/
            return getFile(filename);
        }        
    } else if ( strcmp(filename, "app/public") == 0 ) {
        return "INDEX";
    } else {
        return "404";
    }
}


char *handlerPOST(struct URI u) {
    return "I GOT A POST";
}
