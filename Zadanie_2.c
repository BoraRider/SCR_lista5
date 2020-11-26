#include <stdio.h> 
#include <unistd.h> 
#include <fcntl.h> 

#define BUFOR 256

int main() {

    int pid;
    int fd[2], in_fd, n;
    char buf[BUFOR];
    char nazwa[32];

    if(pipe(fd) < 0) {

        fprintf(stderr, "Nie udalo sie utworzyc potoku\n");
        return 1;
    }

    pid = fork();

    if(pid == 0) {
        //Proces dziecka             
                            // 0 1-fd[0] 2-fd[1]
        close(fd[1]);       // 0 1
        close(0);           // 1
        dup(fd[0]);         // 0-fd[0] 1
        close(fd[0]);       // 0

        execlp("display", "display", NULL);
        
    }
    //        __________________
    // fd[0] / \                \ fd[1] 
    //       \_/________________/
    //
    else {
        //proces rodzica
        close(fd[0]);
        printf("Podaj nazwe pliku\n");
        scanf("%s", nazwa);

        if((in_fd = open(nazwa, O_RDONLY)) < 0) {
            
            fprintf(stderr, "Nie udalo sie otworzyc pliku\n");
            return 2;
        }
        
        while((n = read(in_fd, &buf, BUFOR)) > 0) {
    
            if(write(fd[1], &buf, n) < 0) {
                fprintf(stderr, "Nie udalo sie zapisac do potoku\n");
                return 3;
            }
        }
        close(in_fd);
        sleep(2);
    } 
}