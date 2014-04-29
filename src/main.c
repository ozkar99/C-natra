#include <stdlib.h>
#include <stdint.h>

#include "server.h"

int main(int argc, char *argv[]) {
    server_run("127.0.0.1", 30001);
    
    return EXIT_SUCCESS;
}
