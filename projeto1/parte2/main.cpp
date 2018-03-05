#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>

int numberSystemProcess() {
    return system("ps aux | wc -l");
}


int main (void) {

    std::cout << numberSystemProcess() << std::endl;

    return 0;

}