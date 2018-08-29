// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>  
int main(int argc, char const *argv[])
{
    struct sockaddr_in address;
    int sock = 0, new = 0;
    char ans = 'y';
    struct sockaddr_in serv_addr;
    char command[6] = "ls -1\n";
    char buffer[1024];
    char filename[100];
    char content[10000];
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    if ((new = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }  

    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(&address, '0', sizeof(address));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons( 21 );

    address.sin_family = AF_INET;
    address.sin_port = htons(20);
      
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
   
    if(inet_pton(AF_INET, "127.0.0.1", &address.sin_addr)<=0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }

    if (connect(new, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    send(sock,command,sizeof(command),0);
    recv(new,buffer,sizeof(buffer),0);
    printf("%s\n",buffer);
   
    while(ans == 'y'){
    printf("\nEnter a filename from the above list of files\n");
    memset(filename,'\0',sizeof(filename));
    scanf("%s",filename);
    char display[30] = "cat ";
    strcat(display,filename);
    strcat(display,"\n");
    
    memset(content,'\0',sizeof(content));

    send(sock,display,sizeof(display),0);
    recv(new,content,sizeof(content),0);
    printf("\n%s",content);
    printf("\nDo you want to access more files?\n");
    scanf(" %c", &ans);
    }
    close(new);
    close(sock);
    return 0; 
}
