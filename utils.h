#include<stdlib.h>

/*dem log writting function*/
int logWrite(char *s) {

    /*open the log get on the floor...*/
    FILE *logfd = fopen("log/info.txt", "a+");

    if (logfd == NULL) {
        perror("cant open log!");
    } else {
        /*everybody writes the dinosaur~*/
        printf("%s\n", s);
        fprintf(logfd, "%s\n", s);
        fclose(logfd);
    }

    return 0;
}
