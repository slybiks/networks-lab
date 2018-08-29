
// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include"response.h"
#define PORT 8080
  
int main(int argc, char const *argv[])
{
    struct sockaddr_in address;
    int sock = 0;
    struct sockaddr_in serv_addr;
    char command[10] = "ls -a";
    char filename[50];
    char lastmodified[100];
    char date[100];
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
   system(command);
   printf("\nEnter a file name from the above  list of files\n");
   scanf("%s",filename);
   send(sock,&filename,sizeof(filename),0);
   memset(lastmodified,'\0',sizeof(lastmodified));
   strcat(lastmodified,"stat ");
   strcat(lastmodified,filename);
   strcat(lastmodified," -c %y");
   char lm[70];
   memset(lm,'\0',sizeof(lm));
   FILE *p = popen(lastmodified,"r");
   fgets(lm,sizeof(lm),p);
   FILE *fp = popen("date","r");
   fgets(date,sizeof(date),fp);
   char length[50];
   memset(length,'\0',sizeof(length));
   strcat(length,"stat ");
   strcat(length,filename);
   strcat(length," -c %s");
   char buffer[3];
   FILE *f = popen(length,"r");
   fgets(buffer,sizeof(buffer),f);
   struct response res = {"HTTP/1.1 200 OK", "localhost", "text","Closed"};
   printf("\n%s\n Date: %s\n Server: %s\n Last-Modified: %s\n Content-Length: %s\n Content-Type: %s\n Connection: %s",res.header,date,res.server,lm,buffer,res.contenttype,res.connection);


  return 0;   
}
