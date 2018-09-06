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
    servaddr.sin_port = htons(8085);

    bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr));

    int len, n, m;
    char abspath[100],parent[100],contents[5000],display[100],parentpath[100],find[100],filename[100],buffer[200];

    memset(abspath,'\0',sizeof(abspath));
    memset(parent,'\0',sizeof(parent));
    memset(contents,'\0',sizeof(contents));
    memset(parentpath,'\0',sizeof(parentpath));
    memset(find,'\0',sizeof(find));
    memset(filename,'\0',sizeof(filename));
    memset(display,'\0',sizeof(display));
    memset(buffer,'\0',sizeof(buffer));

    n = recvfrom(sockfd,abspath, sizeof(abspath), MSG_WAITALL, ( struct sockaddr *) &cliaddr,&len);
    m = recvfrom(sockfd,filename, sizeof(filename), MSG_WAITALL, ( struct sockaddr *) &cliaddr,&len);

    strtok(abspath,"\n");

    strcat(parent,"echo \"$(dirname \"");
    strcat(parent,abspath);
    strcat(parent,"\")\"");

    FILE *fp = popen(parent,"r");
    fgets(parentpath,sizeof(parentpath),fp);
    close(fp);

    strtok(parentpath,"\n");

    strcat(find,"find ");
    strcat(find,parentpath);
    strcat(find," -iname ");
    strcat(find,filename);
    strcat(find," -type f -print -quit");

    if( find[0] != '\0' ){

            strcat(display,"cat ");
            strcat(display,parentpath);
            strcat(display,"/");
            strcat(display,filename);
            FILE *p = popen(display,"r");
            while( !feof(p) ){
            fgets(buffer,sizeof(buffer),p);
            strcat(contents,buffer);
	    memset(buffer,'\0',sizeof(buffer));
            }
            close(p);

    }
    sendto(sockfd,contents,sizeof(contents), MSG_CONFIRM, (const struct sockaddr *) &cliaddr,len);
    close(sockfd);
   }
	    
