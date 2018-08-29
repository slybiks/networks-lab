// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include<fcntl.h>
#include"request.h"
#define PORT 8080
int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1,length,i=0, count;
    int addrlen = sizeof(address);
    char filename[50];
    char command[50];
    char user[30];
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
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
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
      
    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, 
                                 sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
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
    memset(command,'\0',sizeof(command));
    recv(new_socket,filename,sizeof(filename),0);
    strcat(command,"stat ");
    strcat(command,filename);
    strcat(command," -c %U");
    strcat(filename,"/HTTP1.1");
    FILE *fp = popen(command,"r");
    fgets(user,sizeof(user),fp);
    close(fp); 
    struct request req = { "localhost", "en-us", "UTF-8", "Keep-Alive"};
    printf("\nGET /%s\nUser-agent : %s\nHost: %s\nAccept-Language : %s\nAccept-encoding: %s\nConnection: %s",filename,user,req.host,req.acceptlang,req.acceptenc,req.connection);
    return 0;
}
