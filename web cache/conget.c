// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <fcntl.h>
#include "request.h"
#include "response.h"
#include "condition.h"
int main()
{
    int sock;
    struct sockaddr_in serv_addr;
    int opt = 1;
    char filename[50];
    char command[200];
    char user[30];
    
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8088);

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

    struct get g;
    recvfrom(sock, &g, sizeof(g), 0, (struct sockaddr*)&serv_addr, (socklen_t *)&serv_addr);

    memset(command,'\0',sizeof(command));
    strcat(command,"stat ");
    strcat(command,g.filename);
    strcat(command," -c %U");

    FILE *rp = popen(command,"r");
    fgets(user,sizeof(user),rp);
    close(rp);

    struct request req = { "localhost", "en-us", "UTF-8", "Keep-Alive"};
    printf("\nGET /%s/HTTP1.1\nUser-agent : %sHost: %s\nAccept-Language : %s\nAccept-encoding: %s\nConnection: %s\n",g.filename,user,req.host,req.acceptlang,req.acceptenc,req.connection);
    
   //response from origin server to proxy server
   char lastmodified[100];
   memset(lastmodified,'\0',sizeof(lastmodified));

   char store_script[200];
   memset(store_script,'\0',sizeof(store_script));

   strcat(store_script,"stat $(find / ! -path \"/");
   strcat(store_script,"*`basename \"$PWD\"`/");
   strcat(store_script,"*\" -iname ");
   strcat(store_script,g.filename);
   strcat(store_script," -type f 2>/dev/null -print -quit) -c %y");
   
    
   FILE *c = popen(store_script,"r");
   fgets(lastmodified,sizeof(lastmodified),c);
   close(c);
   
 
   //response to proxy server  

   struct response res;
   strcpy(res.lm,lastmodified);
  
   sendto(sock, &res, sizeof(res), 0, (struct sockaddr*)&serv_addr,sizeof(serv_addr));

   return 0;
}
