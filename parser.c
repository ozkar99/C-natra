#include<stdlib.h>
#include "utils.h"

struct serverURI {
    char *path[];    
    char *vals[];
};

struct serverMessage {

    struct serverURI URI;
    char *method;
    char *prot;
    
};




struct serverURI serverParseUri(char *s) {
    struct serverURI URI;
    /*parse this shit*/
    return URI;
}
