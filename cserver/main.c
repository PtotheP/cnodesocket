/*
 * Autor: Philipp Seitz
 * Contact: seitz.philipp@outlook.com
 * C-Server waits for incoming connections and handles the input
 */

#include<stdio.h>
#include<stdlib.h>      //strtol
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<string.h>
#include<unistd.h>    //write


int messaging(int new_socket);
int handle(char shandle[]);
int control(char command[]);
 
int main(int argc , char *argv[])
{

    int socket_desc, new_socket, c;
    struct sockaddr_in server, client;

    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
     
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 5000 );
     
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        puts("bind failed");
    }
    puts("bind done");
    
    //Listen
    listen(socket_desc , 3);
    
    //Accept and incoming connection
    puts("Waiting for incoming connections...\n");
    c = sizeof(struct sockaddr_in);
    while( (new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) ){
        
        if (new_socket<0)
        {
            perror("accept failed");
        }
        puts("Connection accepted");
        puts("*************************************\n");
        

        while(messaging(new_socket));
        close(new_socket);
    }
    
     
    return 0;
}


int messaging(int new_socket){
    
            char message[2000], server_reply[2000];
            
            memset(server_reply, 0, 2000);
            memset(message, 0, 2000);
            
            //Receive a reply from client
            //recv is blocking
            if( recv(new_socket, server_reply , 2000 , 0) <= 0){   
                puts("recv failed");
                return 0;
            }
            
            puts("Reply received:");
            puts(server_reply);
            if(handle(server_reply)<= 0){
                puts("Could not handle received DATA");
                strcpy(message, "Could not handle received DATA");
            }else{
                strcpy(message, "DATA handled");
            }
            

            //Reply to the client

            if(write(new_socket , message , strlen(message)) < 0){
                puts("COULD NOT WRITE");
            }
            
            puts("______________________________\n");
            return 1;
}


int handle(char shandle[]){
    //handle the incoming stream
    return 1;
}