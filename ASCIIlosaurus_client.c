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
#include <ncurses.h>
#include "ASCIIlosaurus_world.h"

 
#ifdef NOISY_DEBUG 
# define NOISY_DEBUG_PRINT fprintf(stderr, "%s %s %d\n", __FILE__, __func__, __LINE__)
#else // NOISY_DEBUG
# define NOISY_DEBUG_PRINT
#endif // NOISY_DEBUG

#define OPTIONS "P:H:vh"

static int v = 0;

int main(int argc, char * argv[]) {

    int port = DEFAULT_PORT; //server port
    char * ip_addr = NULL; //ip address
    int run = 1; //keeps game loop running
    int sockfd = 0; //socket file descriptor
    //contains destination IP, destination port, protocol family 
    struct sockaddr_in server; //server address struct
    int input; //key pressed by user

    world_state_t world; //world state struct
    

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

    //create socket, attach to socket file descriptor 
    //call socket with domain: IPv4 networking, type: UDP, default protocol
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd < 0) {
        fprintf(stderr, "socket() failed\n");
		exit(EXIT_FAILURE);
    }
    
    //initalize world struct
    memset(&world, 0, sizeof(world));

    //initalize server address struct
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    //use to convert numbers to big endian with host to network short, before sending
    server.sin_port = htons(port);  

    //convert IP address string to binary
    //store in server struct
    if (inet_pton(AF_INET, ip_addr, &server.sin_addr) <= 0) {
        fprintf(stderr, "invalid IP address\n");
        exit(EXIT_FAILURE);
    }
     
    //start ncurses ui
    setup_ui();


    //start game loop
    while (run) {

        //what data type is input, and where am I getting input from?  and what do I do with the int returned?
        //returns key pressed
        //returns ERR if no key is pressed
        input = get_input();
        NOISY_DEBUG_PRINT;


        if(input != ERR) {

            NOISY_DEBUG_PRINT;
            //socket, data sent, message size, flags, destination, address size
            sendto(sockfd, &input, sizeof(int), 0, (struct sockaddr *)&server, sizeof(server));
            
            if (input == 'q') {
                run = 0;
                break;
            }
        }
        //read from, store data to, size, flags, address, address length
        //recvfrom(sockfd, &world, sizeof(world), 0, NULL, NULL);
        recvfrom(sockfd, &world, sizeof(world), MSG_DONTWAIT, NULL, NULL);

        draw_world(&world);

    }

    //teardown
    teardown_ui();


    exit(EXIT_SUCCESS);
}


