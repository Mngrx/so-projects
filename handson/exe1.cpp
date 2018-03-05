#include <cstdio>
#include <sys/types.h>
#include <unistd.h>

int main () {

    pid_t idPai;

    pid_t idProcesso;

    idPai = getppid();

    idProcesso = getpid();

    printf("Comando: ps -c $pid ou ps -c $ppid\n");
    printf("PID: \t %d \nPPID \t %d \n", idProcesso, idPai);

    while(1) {}

}