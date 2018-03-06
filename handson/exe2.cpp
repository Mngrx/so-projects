#include <cstdlib>
#include <cstdio>
#include <sys/types.h>
#include <unistd.h>

int main () {

    pid_t idProcesso;

    idProcesso = fork();

    switch(idProcesso) {
        case -1:
            break;
        case 0:
            printf("Sou processo filho iD: %d, pai ID: %d\n", getpid(), getppid());
            break;
        default:
            printf("Sou processo pai com iD: %d, pai ID: %d, filho ID: %d\n", getpid(), getppid(), idProcesso);
            break;
    }

    return 0;

}