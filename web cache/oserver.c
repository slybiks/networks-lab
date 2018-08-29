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

    recv(sock,filename,sizeof(filename),0);
    
    char execute[100];
    char find_result[100];
    char temp[100];

    memset(command,'\0',sizeof(command));
    memset(execute,'\0',sizeof(execute));
    memset(find_result,'\0',sizeof(find_result));
    memset(temp,'\0',sizeof(temp));

    strcat(execute,"find / -iname ");
    strcat(execute,filename);
    strcat(execute," -type f 2>/dev/null -print -quit");
    
    FILE *r = popen(execute,"r");
    fgets(temp,sizeof(temp),r);
    close(r);

    strcat(find_result,"`");
    strcat(find_result,execute);
    strcat(find_result,"`");

    strcat(command,execute);
    strcat(command," | awk -F/ '{print $3}'" );
    
    strcat(filename,"/HTTP1.1");

    FILE *fp = popen(command,"r");
    fgets(user,sizeof(user),fp);
    close(fp);

    struct request req = { "localhost", "en-us", "UTF-8", "Keep-Alive"};
    printf("\nGET /%s\nUser-agent : %sHost: %s\nAccept-Language : %s\nAccept-encoding: %s\nConnection: %s\n",filename,user,req.host,req.acceptlang,req.acceptenc,req.connection);
    
   //response from origin server to proxy server
   char lastmodified[100];
   char date[100];

   memset(date,'\0',sizeof(date));
   memset(lastmodified,'\0',sizeof(lastmodified));

   strcat(lastmodified,"stat ");
   strcat(lastmodified,find_result);
   strcat(lastmodified," -c %y");

   char lm[100];
   memset(lm,'\0',sizeof(lm));

   FILE *p = popen(lastmodified,"r");
   fgets(lm,sizeof(lm),p);

   FILE *q = popen("date","r");
   fgets(date,sizeof(date),q);

   char length[50];
   memset(length,'\0',sizeof(length));

   strcat(length,"stat ");
   strcat(length,find_result);
   strcat(length," -c %s");

   char buffer[30];
   FILE *f = popen(length,"r");
   fgets(buffer,sizeof(buffer),f);

   struct response res; 
   strcpy(res.header, "HTTP/1.1 200 OK");
   strcpy(res.date, date);
   strcpy(res.server,"localhost");
   strcpy(res.lm,lm);
   strcpy(res.contentlength,buffer);
   strcpy(res.contenttype,"text");
   strcpy(res.connection,"Closed");
   strcpy(res.path,temp);
  
   sendto(sock, &res, sizeof(res), 0, (struct sockaddr*)&serv_addr,sizeof(serv_addr));
    
   return 0;
}
