
// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#define PORT 8080
  
int main(int argc, char const *argv[])
{
    struct sockaddr_in address;
    int sock = 0, i=0, length,filelength;
    struct sockaddr_in serv_addr;
    char command[7] ="ls -a";
    char lines[15] = "ls -a | wc -l\n";
    char size[10];
    char nof[100];
    char execute[20] = "cat ";
    char catresult[100000];
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
   FILE *fp = popen(lines,"r");
   fgets(lines,sizeof(lines),fp);
   length = atoi(lines);
   char buffer[length][300];
   send(sock , command , strlen(command) , 0 );
   read(sock , buffer, sizeof(buffer));
   printf("\nThe list of files are\n");
   for(; i < length ; i++)
	   printf("%s",*(buffer + i));
   printf("\nEnter the name of the file to be printed from the given list of files\n");
   scanf("%s",nof); 
   strcat(execute,nof);
   send(sock , execute, strlen(execute) , 0);
   read(sock, catresult, sizeof(catresult));
   printf("\n\n%s\n",catresult);
   return 0;
}
