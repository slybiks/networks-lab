// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include<fcntl.h>
#define PORT 8080
#define MAX 100000
int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1,length,i=0, count;
    int addrlen = sizeof(address);
    char command[14];
    char size[14] = "";
    char lof[6]; 
    char catcommand[20];
    char catresult[1000];
    char final[1000000];
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
    recv(new_socket , command , 5 , 0 );
    command[5]='\0';
    strcat(lof,command);
    strcat(command, " | wc -l");
    command[14]='\0';
    strcpy(size, command);
    size[14]='\0';
    FILE *fp = popen(size,"r"); 
    fgets(size, sizeof(size), fp);
    length = atoi(size);
    pclose(fp);
    char buffer[length][300];
    char response[length][300];
    lof[5]='\0';
    FILE *f = popen(lof,"r");
    for(; i < length ; i++)
    {
      fgets(buffer[i],300, f);
      strcat(response[i],buffer[i]);
    }	    
    printf("\nThe list of files are being displayed on the client side\n");
    write(new_socket,response,sizeof(response));
    recv(new_socket , catcommand , sizeof(catcommand)-1, 0);
    count = strlen(catcommand);
    catcommand[count]='\0';
    FILE *fd = popen(catcommand,"r");
    while(!feof(fd)){
    fgets(catresult, sizeof(catresult), fd);
    strcat(final,catresult);
    } 
    printf("\nThe requested file is being displayed on the client side\n");
    write(new_socket, final, strlen(final)); 
    return 0;
}
