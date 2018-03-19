#include <iostream>
#include <cstdlib>
#include <sstream>  
#include <unistd.h>
#include <sys/types.h>
#include <vector>
#include <string.h>
#include <chrono>
#include <thread>

using namespace std;
using namespace chrono;
using namespace this_thread;
/*
usar para dar o wait 
sleep_until(system_clock::now() + seconds(3));
*/

   
std::string exec(char *cmd);
void menuSystem();
void blinkLed(int led1, int led2, int led3);
void ledHigh(int led);
void ledLow(int led);
bool interruption();
void killProcess(string pid);
void gpioReady(int pin, string direction);


int main () {

	menuSystem();
    
    return 0;
}

// Função que executa um comando no terminal e retorna o que seria printado
// como string
// Creditos:
// https://stackoverflow.com/questions/32039852/returning-output-from-bash-script-to-calling-c-function
std::string exec(char *cmd)
{
    FILE *pipe = popen(cmd, "r");
    if (!pipe)
        return "ERROR";
    char buffer[128];
    std::string result = "";
    while (!feof(pipe))
    {
        if (fgets(buffer, 128, pipe) != NULL)
            result += buffer;
    }
    pclose(pipe);
    return result;
}

