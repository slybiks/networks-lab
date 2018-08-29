// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
int main(int argc, char const *argv[])
{
    int server_fd, new_socket, server, new_connection;
    struct sockaddr_in address, new_server;
    int opt = 1;
    int addrlen = sizeof(address);
    int newserverlen = sizeof(new_server);
    char command[6] = {0};
    char display[30] = {0};
    char buffer[1024] = {0};
    char result[1024] = {0};
    char content[10000];
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

   if ((server  = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
      
    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                  &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                  &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( 21 );

    new_server.sin_family = AF_INET;
    new_server.sin_addr.s_addr = INADDR_ANY;
    new_server.sin_port = htons(20 );
      
    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, 
                                 sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (bind(server, (struct sockaddr *)&new_server,
                                 sizeof(new_server))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    if (listen(server, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }


    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
                       (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    if ((new_connection = accept(server, (struct sockaddr *)&new_server,
                       (socklen_t*)&newserverlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    
    recv(new_socket,command,sizeof(command),0);
    printf("\nReceived request for listing files on port 21\n");
    FILE *fp = popen(command,"r");
    while( !feof(fp) ){
    fgets(buffer,sizeof(buffer),fp);
    strcat(result,buffer);
    }
    close(fp);
    printf("\nSending back list of files on port 20\n");
    send(new_connection,result,strlen(result),0);


    while(recv(new_socket,display,sizeof(display),0)){
    memset(buffer,'\0',sizeof(buffer));
    memset(content,'\0',sizeof(content));
  
    printf("Request received on port 21 for : %s",display);

    FILE *f = popen(display,"r");
    while( !feof(f) ){
    fgets(buffer,sizeof(buffer),f);
    strcat(content,buffer);
    }
    close(f);

    memset(display,'\0',sizeof(display));
    printf("\nSending back file content on port 20\n");
    send(new_connection,content,strlen(content),0);
    }
    close(new_connection);
    close(new_socket);
    return 0;
}
