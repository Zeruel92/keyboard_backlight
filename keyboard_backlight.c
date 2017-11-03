#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(){
        pid_t child;
        int fd;
        int mode = (S_IRUSR | S_IWUSR);

        if((fd=open("/tmp/keyboard.lock", O_CREAT|O_TRUNC|O_EXCL, mode))>0) {
                printf("File lock creato\n");
                if((child = fork())< 0) {
                        printf("Errore nella fork\n");
                }
                else if (child==0) {
                        printf("Accensione led tastiera\n");
                        if( execlp("xset","xset","led","3",(char *)0) < 0) {
                                printf("error executing xset\n");
                        }
                }
        }
        else{
                printf("File lock già esistente\n");
                if((child = fork())< 0) {
                        printf("Errore nella fork\n");
                }
                else if (child==0) {
                        printf("Spegnimento dei led\n");
                        pid_t pid= fork();
                        if(pid==0) {
                                if( execlp("xset","xset","-led","3",(char *)0) < 0) {
                                        printf("error executing xset");
                                }
                        }
                        waitpid(pid,NULL,0);
                        printf("removing lock file\n");
                        int ret=remove("/tmp/keyboard.lock");
                        exit(ret);
                }
        }
        waitpid(child,NULL,0);
        exit(0);
}
