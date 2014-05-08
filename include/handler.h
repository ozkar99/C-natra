#ifndef HANDLER_H
#define HANDLER_H

/*this should return the html when given the uri as parameter*/
char *handlerUri(struct URI u);

/*handle GET request*/
char *handlerGET(struct URI u);

/*handle POST requests*/
char *handlerPOST(struct URI u);

#endif
