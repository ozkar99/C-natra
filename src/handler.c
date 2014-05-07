#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "parser.h"
#include "handler.h"


/*This should return the html page, when given an uri as parameter*/
char *handlerUri(struct URI u) {
    if ( strncmp(u.method, "GET", 3) == 0 ) {
        return "I GOT A GET"; 
    } else if ( strncmp(u.method, "POST", 4) == 0 ) {
        return "I GOT A POST";
    } else {
        return "I GOT SOMETHING ELSE";
    }
}
