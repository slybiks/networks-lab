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

#define PORT 8080
int main(int argc, char const *argv[])
{
    int server_fd, new_socket,server,new_ossocket;
    struct sockaddr_in address,osaddress;
    int opt = 1;
    int addrlen = sizeof(address);
    int osaddrlen = sizeof(osaddress);
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

    //Origin server call

    if ((server = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8088
    if (setsockopt(server, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                  &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    osaddress.sin_family = AF_INET;
    osaddress.sin_addr.s_addr = INADDR_ANY;
    osaddress.sin_port = htons(8088 );

    // Forcefully attaching socket to the port 8088
    if (bind(server, (struct sockaddr *)&osaddress,
                                 sizeof(osaddress))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_ossocket = accept(server, (struct sockaddr *)&osaddress,
                       (socklen_t*)&osaddrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    recv(new_socket,filename,sizeof(filename),0);
    
    char execute[100];
    char find_result[200];
    char psfile[100];
    char psfilename[100];
    
    memset(command,'\0',sizeof(command ));
    memset(execute,'\0',sizeof(execute));
    memset(find_result,'\0',sizeof(find_result));
    memset(psfile,'\0',sizeof(psfile));
    memset(psfilename,'\0',sizeof(psfilename));

    strcat(psfile,filename);
    strcat(psfilename,filename);

    strcat(execute,"find . -name ");
    strcat(execute,filename);
    strcat(execute," -type f");

    FILE *f = popen(execute,"r");
    fgets(find_result,sizeof(find_result),f);
    close(f);


   char lastmodified[100];
   char date[100]; 
   

   //If requested object exists in proxy server 
    if( find_result[0] != '\0' ){

    strcat(command,"stat ");
    strcat(command,filename);
    strcat(command," -c %U");

    strcat(filename,"/HTTP1.1");

    FILE *fp = popen(command,"r");
    fgets(user,sizeof(user),fp);
    close(fp);
                             
    char ifmodified[100];
    char modify[100];
    memset(ifmodified,'\0',sizeof(ifmodified));
    memset(modify,'\0',sizeof(modify));
    
    strcat(ifmodified,"stat ");
    strcat(ifmodified,psfile);
    strcat(ifmodified," -c %y");

    FILE *rp = popen(ifmodified,"r");
    fgets(modify,sizeof(modify),rp);
    close(rp);

    struct request req = { "localhost", "en-us", "UTF-8", "Keep-Alive"};
    printf("\nGET /%s\nUser-agent : %s\nHost: %s\nAccept-Language : %s\nAccept-encoding: %s\nConnection: %s\n",filename,user,req.host,req.acceptlang,req.acceptenc,req.connection);

    printf("\nGET %s If-modified-since : %s\n",filename,modify);
    struct get g;
    strcpy(g.filename,psfile);
    strcpy(g.date,modify);

    sendto(new_ossocket, &g, sizeof(g), 0, (struct sockaddr*)&osaddress,sizeof(osaddress));

    struct response con_get;
    recvfrom(new_ossocket, &con_get, sizeof(con_get), 0, (struct sockaddr*)&osaddress, (socklen_t *)&osaddress);
    
    strtok(con_get.lm," ");
    strtok(modify," ");
    strtok(con_get.lm,"\n");
    strtok(modify,"\n");

    
    if(strcmp(con_get.lm,modify) != 0){

    char run_script[100];
    memset(run_script,'\0',sizeof(run_script));
    
    char store_script[200];
    memset(store_script,'\0',sizeof(store_script));
    
    strcat(store_script,"find / ! -path \"/");
    strcat(store_script,"*`basename \"$PWD\"`/");
    strcat(store_script,"*\" -iname ");
    strcat(store_script,psfilename);
    strcat(store_script," -type f 2>/dev/null -print -quit");


    FILE *d = popen(store_script,"r");
    fgets(run_script,sizeof(run_script),d);
    close(d);

    char pwd[100];
    char source[100];
    char copyfile[200];

    memset(pwd,'\0',sizeof(pwd));
    memset(source,'\0',sizeof(source));
    memset(copyfile,'\0',sizeof(copyfile));
    
    strtok(run_script,"\n");
    strcat(source,run_script);

    FILE *a = popen("pwd","r");
    fgets(pwd,sizeof(pwd),a);
    close(a);

    strtok(pwd,"\n");
    strcat(pwd,"/");

    strcat(pwd,psfilename);

    strcat(copyfile,"cp ");
    strcat(copyfile,source);
    strtok(copyfile,"\n");

    strcat(copyfile," ");
    strcat(copyfile,pwd);

    system(copyfile);

   }
	    
   send(new_socket,&psfilename,sizeof(psfilename),0);
    
   }
  
   // If requested object does not exist in the proxy server 
   
   //Proxy sends request to origin server to fetch file
   if( find_result[0] == '\0'){ 

    // Socket to call origin server  		
	
   struct response res;
   send(new_ossocket,&filename,sizeof(filename),0);	
   //Proxy receives object from origin server    
   recvfrom(new_ossocket, &res, sizeof(res), 0, (struct sockaddr*) &osaddress, (socklen_t *)&osaddress);

   printf("%s\nDate: %s\nServer: %s\nLast-Modified: %s\nContent-Length: %s\nContent-Type: %s\nConnection: %s\n",res.header,res.date,res.server,res.lm,res.contentlength,res.contenttype,res.connection);

   char path[100];
   char copy[200];
   char curdir[100];
   
   memset(path,'\0',sizeof(path));
   memset(copy,'\0',sizeof(copy));

   strcat(path,res.path);
   
   FILE *a = popen("pwd","r");
   fgets(curdir,sizeof(curdir),a);
   close(a);

   strtok(curdir,"\n");
   strcat(curdir,"/");
 
   strcat(curdir,psfile);

   strcat(copy,"cp ");
   strcat(copy,path);
   strtok(copy,"\n");
   
   strcat(copy," ");
   strcat(copy,curdir);

   system(copy);

   send(new_socket,&path,sizeof(path),0);

   }

    return 0;

}
