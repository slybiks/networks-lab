// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include "datagram.h"
#define PORT 8080
  
int main(int argc, char const *argv[])
{
    struct sockaddr_in address;
    int sock = 0;
    struct sockaddr_in serv_addr;
    struct datagram data;
    struct datagram serverdata = {"Bhavi",2016103511,"cutebhavi@gmail.com"};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
  
    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
      
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
  
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    sendto(sock, &serverdata, sizeof(serverdata), 0, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
    printf("Data sent to server\n");
    printf("Data received is\n");
    recvfrom(sock, &data, sizeof(data), 0, (struct sockaddr*) &serv_addr, (socklen_t *)&serv_addr);
    printf("%s\n%ld\n%s\n",data.name,data.regno,data.mail);
    return 0;
}
