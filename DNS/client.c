// Client side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>
 
int main() {
    int sockfd;
    struct sockaddr_in servaddr;
 
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
 
    memset(&servaddr, 0, sizeof(servaddr));
     
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(8080);
    servaddr.sin_addr.s_addr = INADDR_ANY;
     
    int n,len;

    char filename[100];
    char contents[500];

    memset(filename,'\0',sizeof(filename));
    memset(contents,'\0',sizeof(contents));

    printf("\nEnter a filename\n");
    scanf("%s",filename);
     
    sendto(sockfd,filename,strlen(filename), MSG_CONFIRM, (const struct sockaddr *) &servaddr,sizeof(servaddr));
    n = recvfrom(sockfd,contents,sizeof(contents), MSG_WAITALL,(struct sockaddr *)&servaddr,&len);
    printf("%s\n",contents);
    return 0;

}
