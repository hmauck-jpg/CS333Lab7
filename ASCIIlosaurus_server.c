// Haleah Mauck
// 3/9/2026
// CS-314-006
// Lab7 ASCIIlosaurus
// hmauck@pdx.edu
// this is the implementation file for the ASCIIlosaurus_server program
 
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
 
#define OPTIONS "P:vh"


static int v = 0;

int main(int argc, char * argv[]) {

    int port = DEFAULT_PORT; 
    int sockfd = 0; //socket file descriptor 
    struct sockaddr_in server; 
    
    struct sockaddr_in client;
    socklen_t clientLen = sizeof(client);

    struct sockaddr_in clients[MAX_PLAYERS];

    world_state_t world; //server's world state struct

    int run = 1; //keep server running
    int ch; //the ascii character recived from keyboard

    int playerIndex = -1;

    //seed rand function with time
    srand(time(NULL));

    {
         int opt = -1;
        while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
            switch (opt) {
                case 'P':
                     sscanf(optarg, "%d", &port);
                     break;
                case 'v':
                    v = 1;
                    break;
                case 'h':
                    printf("ASCIIlosaurus_server [P:vh]\n");
                    printf("        -P #    port on which to send/receive packets (default is 10011)\n");
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

    //create socket, attach to socket file descriptor 
    //call socket with domain: IPv4 networking, type: UDP, default protocol
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd < 0) {
        fprintf(stderr, "socket() failed\n");
		exit(EXIT_FAILURE);
    }

    //initalize server IP address
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = INADDR_ANY; //accept packet from any IP address

    //bind server to port
    bind(sockfd,(struct sockaddr *)&server, sizeof(server));

    //initalize world struct 
    memset(&world, 0, sizeof(world));

    //initalize clients array
    memset(clients, 0, sizeof(clients));

    //game server loop
    while (run) {

        NOISY_DEBUG_PRINT;
        recvfrom(sockfd, &ch, sizeof(int), 0, (struct sockaddr *)&client, &clientLen);
        NOISY_DEBUG_PRINT;
        if (v) {
            fprintf(stderr,"Received key: %c from %s:%d\n", ch, inet_ntoa(client.sin_addr), ntohs(client.sin_port));
        }


        playerIndex = -1;
        for (int i = 0; i < MAX_PLAYERS && playerIndex == -1; ++i) {
            if (clients[i].sin_addr.s_addr == client.sin_addr.s_addr && clients[i].sin_port == client.sin_port) {
                playerIndex = i;
            }
        }

        //if player doesn't exist in clients 
        if (playerIndex == -1) {
            //create new player 
            for (int i = 0; i < MAX_PLAYERS; ++i) {
                if (world.players[i].active == 0) {
                    //activate player
                    world.players[i].active = 1;

                    //assign random grid position 
                    world.players[i].x = rand() % GRID_W;
                    world.players[i].y = rand() % GRID_H;

                    //assign player unused symbol (@ through X
                    world.players[i].symbol = '@' + i;

                    //record player to game
                    clients[i] = client;

                    playerIndex = i;
                    break;
                }
            }
            
        } 

        if (playerIndex != -1) {
            switch(ch) {

                case 'w':
                case 'k':
                case KEY_UP:
                    world.players[playerIndex].y--;
                    break;

                case 's':
                case 'j':
                case KEY_DOWN:
                    world.players[playerIndex].y++;
                    break;

                case 'a':
                case 'h':
                case KEY_LEFT:
                    world.players[playerIndex].x--;
                        break;

                case 'd':
                case 'l':
                case KEY_RIGHT:
                    world.players[playerIndex].x++;
                    break;

                case 'q':
                    world.players[playerIndex].active = 0;
                    break;
            }

            //wrap the player's position 
            if (world.players[playerIndex].x < 0) {
                world.players[playerIndex].x = GRID_W - 1;
            }
             
            if (world.players[playerIndex].x >= GRID_W) {
                world.players[playerIndex].x = 0;
            }
             
            if (world.players[playerIndex].y < 0) {
                world.players[playerIndex].y = GRID_H - 1;
            }
             
            if (world.players[playerIndex].y >= GRID_H) {
                world.players[playerIndex].y = 0;
            }

        }

        //update the world for all clients
        for (int i = 0; i < MAX_PLAYERS; ++i) {
            if (world.players[i].active) {
                NOISY_DEBUG_PRINT;
                sendto(sockfd, &world, sizeof(world), 0, (struct sockaddr *)&clients[i], sizeof(clients[i]));
                NOISY_DEBUG_PRINT;
            }
             
        }
             
    }

   

    exit(EXIT_SUCCESS);
}

 