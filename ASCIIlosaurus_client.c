// Haleah Mauck
// 3/9/2026
// CS-314-006
// Lab7 ASCIIlosaurus
// hmauck@pdx.edu
// this is the implementation file for the ASCIIlosaurus_client program
 
//valgrind --leak-check=full --show-leak-kinds=all
 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "ASCIIlosaurus_world.h"

 
#ifdef NOISY_DEBUG 
# define NOISY_DEBUG_PRINT fprintf(stderr, "%s %s %d\n", __FILE__, __func__, __LINE__)
#else // NOISY_DEBUG
# define NOISY_DEBUG_PRINT
#endif // NOISY_DEBUG

#define OPTIONS "P:H:vh"

static int v = 0;

int main(int argc, char * argv[]) {

    int port = DEFAULT_PORT;
    //should this be different?  
    char * ip_addr = NULL;
    

    {
         int opt = -1;
        while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
            switch (opt) {
                case 'H':
                     ip_addr = optarg;
                     break;
                case 'P':
                    sscanf(optarg, "%d", &port);
                    break;
                case 'v':
                    v = 1;
                    break;
                case 'h':
                    printf("ASCIIlosaurus_client [P:H:vh]\n");
                    printf("        -H str  hostname ip of server (in IPv4 format) (default 131.252.208.23)\n");
                    printf("        -P #    port on which to contact server (default is 10011)\n");
                    printf("        -v      verbose diagnostics\n");
                    printf("        -h      show help\n"); 
                    exit(EXIT_SUCCESS);
                    break;
                default:
                    exit(EXIT_FAILURE);
                    break;
            }
        }
    }

    if (ip_addr == NULL) {
        ip_addr = "131.252.208.23";
    }

    /*
    setup_ui()

    while(running)
    {
        read keyboard input

        send command to server

        receive world_state from server

        draw_world()
    }

    teardown_ui()
    */




    exit(EXIT_SUCCESS);
}


