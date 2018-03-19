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
void menuSystem(); //menu de usabilidade
void blinkLed(int led1, int led2, int led3); //piscar todos os leds
void valueHigh(int pin); //setar valor de um pino como low
void valueLow(int pin); //setar valor de um pino como low
bool interruption();
void killProcess(string pid);
bool gpioReady(int pin, string direction);
void panic();

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

bool gpioReady(int pin, string direction) {
    
    if (pin < 1) {
        cerr << "Pin Inválido!" << endl;
        return false;
    }

    if (!(direction == "out" || direction == "in")) {
        cerr << "Direção inválida!" << endl;
        return false;
    }

    string _pin = to_string(pin);

    string comando = "echo " + _pin + " > /sys/class/gpio/export";

    system(comando.c_str());

    comando = "echo " + direction + " > /sys/class/gpio/gpio"+_pin+"/direction";

    system(comando.c_str());

    return true;

}

void valueLow(int pin) {
    string _pin = to_string(pin);

    string comando = "echo 0 > /sys/class/gpio/gpio" + _pin + "/value";

    system(comando.c_str());
}
 
void valueHigh(int pin)
{
    string _pin = to_string(pin);

    string comando = "echo 1 > /sys/class/gpio/gpio" + _pin + "/value";
    
    system(comando.c_str());
}

void blinkLed(int led1, int led2, int led3) {

    valueHigh(led1);
    valueHigh(led2);
    valueHigh(led3);
    sleep_until(system_clock::now() + seconds(1));
    valueLow(led1);
    valueLow(led2);
    valueLow(led3);
    sleep_until(system_clock::now() + seconds(1));
}

void panic(string pid, int led1, int led2, int led3) {

    while (interruption()) {
        blinkLed(led1, led2, led3);
    }

    killProcess(pid);

    return;
}

void killProcess(string pid) {

    string comando = "kill -9 " + pid;

    system(comando.c_str());

}

bool interruption() {
    
}