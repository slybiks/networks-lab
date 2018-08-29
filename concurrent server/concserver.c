#include<signal.h>
#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>
#include<sys/types.h>
#include<stdlib.h>

int main()
{

    struct sockaddr_in myaddr ,clientaddr;
    int sockid,newsockid;
    sockid=socket(AF_INET,SOCK_STREAM,0);
    memset(&myaddr,'0',sizeof(myaddr));
    myaddr.sin_family=AF_INET;
    myaddr.sin_port=htons(8080);
    myaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
    if(sockid==-1)
    {
        perror("socket");
    }
    int len=sizeof(myaddr);
    if(bind(sockid,( struct sockaddr*)&myaddr,len)==-1)
    {
        perror("bind");
    }
    if(listen(sockid,10)==-1)
    {
        perror("listen");
    }
    int pid,new;
    static int counter=0;
    for(;;)
    {
        new = accept(sockid, (struct sockaddr *)&clientaddr, &len);

        if ((pid = fork()) == -1)
        {
            close(new);
            continue;
        }
        else if(pid > 0)
        {
            close(new);
            counter++;
            printf("\nIn parent process\n");
            continue;
        }
        else if(pid == 0)
        {
            char buf[100];
            char s[10000];
            counter++;
            printf("\nCreating child server process\n");
            printf("\nClient %d is connected\n",counter);
            recv(new,s,10000,0);
            printf("%s",s);
            snprintf(buf, sizeof buf, "Hello client %d", counter);
            send(new, buf, strlen(buf), 0);
            close(new);
            break;
        }
}
printf("\nServer responded\n");
    close(sockid);
    return 0;
}
