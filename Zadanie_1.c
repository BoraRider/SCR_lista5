#include <stdio.h> 
#include <unistd.h> 
#include <fcntl.h> 

#define BUFOR 8

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
        close(fd[1]);
        while( read(fd[0], &buf, 8)>0){
            write(STDOUT_FILENO, "#", 1);
            write(STDOUT_FILENO, &buf, 8);
            write(STDOUT_FILENO, "#", 1);
        }

        close(fd[0]);

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
    } 
    printf("\n");
}