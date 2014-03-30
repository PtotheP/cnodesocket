/*
 * Autor: Philipp Seitz
 * Contact: seitz.philipp@outlook.com
 * C-Server waits for incoming connections and handles the input
 */

#include<stdio.h>
#include<stdlib.h>      //strtol
#include<sys/socket.h>
#include<arpa/inet.h>   //inet_addr
#include<string.h>
#include<unistd.h>      //write
#include <pthread.h>
#include"tempsensor.h"


int messaging(int new_socket);
int handle(char shandle[]);
int control(char command[]);
void *texting(void *pSocket);
int textingenabled;

int main(int argc, char *argv[]) {

    int socket_desc, socket_con, c;
    struct sockaddr_in server, client;
    pthread_t textthread;
    textingenabled = 1;

    //Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1) {
        printf("Could not create socket");
    }

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(5000);

    //Bind
    if (bind(socket_desc, (struct sockaddr *) &server, sizeof (server)) < 0) {
        puts("bind failed");
    }
    puts("bind done");

    //Listen
    listen(socket_desc, 3);

    //Accept an incoming connection
    puts("Waiting for incoming connections...\n");
    c = sizeof (struct sockaddr_in);
    while ((socket_con = accept(socket_desc, (struct sockaddr *) &client, (socklen_t*) & c))) {

        if (socket_con < 0) {
            perror("accept failed");
        }
        textingenabled = 1;
        puts("Connection accepted");
        puts("*************************************\n");

        pthread_create(&textthread, NULL, texting, (void *) &socket_con);

        while (messaging(socket_con));

        puts("Messaging over waiting for Thread textthread to be finished");
        textingenabled = 0;

        pthread_join(textthread, NULL);

        close(socket_con);

        puts("Connection closed");
        puts("*************************************\n");
    }


    return 0;
}

int messaging(int pSocket) {

    char message[2000], server_reply[2000];

    memset(server_reply, 0, 2000);
    memset(message, 0, 2000);

    //Receive a reply from client
    //recv is blocking
    if (recv(pSocket, server_reply, 2000, 0) <= 0) {
        puts("recv failed");
        return 0;
    }

    puts("Reply received:");
    puts(server_reply);
    if (handle(server_reply) <= 0) {
        puts("Could not handle received DATA");
        strcpy(message, "Could not handle received DATA");
    } else {
        strcpy(message, "DATA handled");
    }


    //Reply to the client

    if (write(pSocket, message, strlen(message)) < 0) {
        puts("COULD NOT WRITE");
    }

    puts("______________________________\n");
    return 1;
}

int handle(char shandle[]) {
    //handle the incoming stream
    return 1;
}

//Function called in textthraed

void *texting(void *pSocket) {

    int textsocket = *(int*) pSocket;
    char message[2000];

    while (textingenabled) {

        memset(message, 0, 2000);
        snprintf(message, 2000, "Temperatur: %d", Temp());

        if (write(textsocket, message, strlen(message)) < 0) {
            puts("COULD NOT WRITE");
            return NULL;
        }
        
        usleep(1000000); //Give the prozessor some Time to do his own stuff..
    }
}