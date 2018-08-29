#include <stdio.h>
#include <fcntl.h>
int main()
{
char buffer[100];
memset(buffer,sizeof(buffer),'\0');
FILE *fp = popen("sh all_dir.sh","r");
fgets(buffer,sizeof(buffer),fp);
close(fp);

printf("%s\n",buffer);
}
