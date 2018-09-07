// Server side implementation of UDP client-server model
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
    struct sockaddr_in servaddr, cliaddr;
     
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
     
    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));
     
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(8080);
     
    bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr));
     
    int len, n;
    char filename[100],abspath[100],find[100],fullpath[100],display[100],contents[500],buffer[100];
    
    memset(filename,'\0',sizeof(filename));
    memset(abspath,'\0',sizeof(abspath));
    memset(fullpath,'\0',sizeof(fullpath));
    memset(find,'\0',sizeof(find));
    memset(display,'\0',sizeof(display));
    memset(contents,'\0',sizeof(contents));
    memset(buffer,'\0',sizeof(buffer));

    n = recvfrom(sockfd, filename, sizeof(filename), MSG_WAITALL, ( struct sockaddr *) &cliaddr,&len);
    
    FILE *f = popen("pwd","r");
    fgets(abspath,sizeof(abspath),f);
    close(f);

    strtok(abspath,"\n");
    
    strcat(find,"find ");
    strcat(find,abspath);
    strcat(find," -iname ");
    strcat(find,filename);
    strcat(find," -type f -print -quit");

    FILE *s = popen(find,"r");
    fgets(fullpath,sizeof(fullpath),s);
    close(s);
    
     if( fullpath[0] != '\0' ){

	    strcat(display,"cat ");
	    strcat(display,fullpath);
	    FILE *p = popen(display,"r");
	    while( !feof(p) ){
	    fgets(buffer,sizeof(buffer),p);
            strcat(contents,buffer);
            memset(buffer,'\0',sizeof(buffer));
            }
	    close(p);

    }	
    else{

	int sockid;
        struct sockaddr_in client;

        sockid = socket(AF_INET, SOCK_DGRAM, 0);

        memset(&client, 0, sizeof(client));

    	client.sin_family = AF_INET;
    	client.sin_port = htons(8088);
    	client.sin_addr.s_addr = INADDR_ANY;

    	int flag,length;

    	sendto(sockid,abspath,strlen(abspath), MSG_CONFIRM, (const struct sockaddr *) &client,sizeof(client));
	sendto(sockid,filename,strlen(filename), MSG_CONFIRM, (const struct sockaddr *) &client,sizeof(client));

    	flag = recvfrom(sockid,(char *)contents,sizeof(contents), MSG_WAITALL, (struct sockaddr *) &client, &length);

	close(sockid);

    }	
    sendto(sockfd,contents,sizeof(contents), MSG_CONFIRM, (const struct sockaddr *) &cliaddr,len);
    return 0;    
     	    
}

