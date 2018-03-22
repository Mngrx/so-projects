#include <iostream>
#include <cstdlib>
#include <sstream>  
#include <unistd.h>
#include <sys/types.h>
#include <vector>
#include <string.h>
#include <chrono>
#include <thread>
#include <csignal>

#define LED_VERDE 50
#define LED_VERMELHO 31
#define LED_AMARELO 48
#define BUTTON 115

using namespace std;
using namespace chrono;
using namespace this_thread;
/*
usar para dar o wait 
sleep_until(system_clock::now() + seconds(3));
*/

/*
* ps -e -o %mem | sort -r
* ps -e -o %cpu | sort -r
*/
   
std::string exec(const char *cmd);
void menuSystem(); //menu de usabilidade
void blinkLed(int led1, int led2, int led3); //piscar todos os leds
void valueHigh(int pin); //setar valor de um pino como low
void valueLow(int pin); //setar valor de um pino como low
bool buttonPress(int pin); //retorna se um botão foi pressionado ou não
void killProcess(string pid); //mata o processo do pid passado
bool gpioReady(int pin, string direction); //prepara a gpio e muda sua direção
bool gpioFinish(int pin); //desaloca a gpio 
void dontpanic(string pid, int led1, int led2, int led3, int button); //ativa o estado de pânico
float totalConsumoCPU(); //captura a somatória do consumo da cpu
float totalConsumoMEMO(); //captura a somatória do consumo da memória
string maiorConsumidor(bool cpuMemo); //captura qual processo está consumindo mais
void monitorar(string tipo); //realiza monitoramento
void signalHandler(int sig); //função para finalizar corretamente o programa


int main()
{

    signal(SIGINT, signalHandler);

    gpioReady(BUTTON, "in");
    gpioReady(LED_AMARELO, "out");
    gpioReady(LED_VERDE, "out");
    gpioReady(LED_VERMELHO, "out");

    menuSystem();
    
    return 0;
}

// Função que executa um comando no terminal e retorna o que seria printado
// como string
// Creditos:
// https://stackoverflow.com/questions/32039852/returning-output-from-bash-script-to-calling-c-function
std::string exec(const char *cmd)
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

void signalHandler(int sig) {

    gpioFinish(LED_VERMELHO);
    gpioFinish(LED_VERDE);
    gpioFinish(LED_AMARELO);
    gpioFinish(BUTTON);

    cout << "Programa finalizado com sucesso!\n" << endl;
    exit(0);
}

bool gpioFinish(int pin) {

    if (pin < 1) {
        cerr << "Pin Inválido!" << endl;
        return false;
    }

    string _pin = to_string(pin);

    string comando = "echo " + _pin + " > /sys/class/gpio/unexport";

    system(comando.c_str());

    return true;
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
    valueLow(led1);
    valueLow(led2);
    valueLow(led3);
}

void dontpanic(string pid, int led1, int led2, int led3, int button) {

    while (1) {
        blinkLed(led1, led2, led3);
        if (buttonPress(button)) {
            break;
        }
    }

    killProcess(pid);

    valueLow(LED_VERDE);
    valueLow(LED_VERMELHO);
    valueLow(LED_AMARELO);
	
    sleep_until(system_clock::now() + seconds(3));


    return;
}

void killProcess(string pid) {

    string comando = "kill -9 " + pid;

    system(comando.c_str());

}

bool buttonPress(int pin) {

    if (pin < 1) {
        return false;
    }

    string _pin = to_string(pin);

    string comando = "cat /sys/class/gpio/gpio" + _pin + "/value";

    string resultado = exec(comando.c_str());

    istringstream reader(resultado);

    int aux;

    reader >> aux;

    if (aux == 1) {
        return true;
    } else {
        return false;
    }
}

void menuSystem()
{
    int c = 0;

    std::cout << std::endl;
    std::cout << "\t Monitoramento" << std::endl;
    std::cout << std::endl;
    std::cout << "1. Monitorar CPU" << std::endl;
    std::cout << "2. Monitorar Memoria" << std::endl;
    std::cout << "0. Sair" << std::endl;

    std::cin >> c;

    switch (c)
    {
    case 1:
        monitorar("CPU");
        break;

    case 2:
        monitorar("MEMORY");
        break;

    case 0:
        exit(0);
        break;
    }
}

float totalConsumoCPU()
{

    string command = "ps h -e -o %cpu --sort=-%cpu";

    string consumoCPU = exec(command.c_str());
    std::istringstream reader(consumoCPU);

    float consumoTotal = 0;
    string line;

    while (reader >> line)
    {
        consumoTotal += atof(line.c_str());
    }
    cout << "Consumo CPU: "<< consumoTotal << endl;
    return consumoTotal;
}

float totalConsumoMEMO()
{

    string command = "ps h -e -o %mem --sort=-%mem";

    string consumoMEMO = exec(command.c_str());
    std::istringstream reader(consumoMEMO);

    float consumoTotal = 0;
    string line;

    while (getline(reader, line))
    {
        consumoTotal += atof(line.c_str());
    }

    cout << "Consumo MEM: " << consumoTotal << endl;

    return consumoTotal;
}

string maiorConsumidor(bool cpuMemo)
{
    /*cpuMemo
	/ TRUE - retorna o pid do maior consumidor da CPU
	/ FALSE - retorna o pid do maior consumidor da Memoria
	*/
    string comando, aux;
    if (cpuMemo) {
        comando = "ps h -e -o pid --sort=-%cpu";
        aux = exec(comando.c_str());
        std::istringstream reader(aux);
        reader >> aux;
        cout << aux;
        return aux;
    } else {
        comando = "ps h -e -o pid --sort=-%mem";
        aux = exec(comando.c_str());
        std::istringstream reader(aux);
        reader >> aux;
        cout << aux;
        return aux;
    }
}

void monitorar(string tipo)
{
    std::cout << std::endl;
    std::cout << "\t Monitoramento "<< tipo << " - Use CTRL C para sair" << std::endl;
    std::cout << ">> Em caso de consumo excessivo utilize o botao do panico." << std::endl;
    std::cout << std::endl;

    float consumo = 0;
    string pid;
    while (1)
    {
        if (tipo.compare("CPU") == 0) {
            consumo = totalConsumoCPU();
        } else if (tipo.compare("MEMORY") == 0) {
            consumo = totalConsumoMEMO();
        }

        if (consumo > 0 && consumo <= 25)
        {
            valueHigh(LED_VERDE);
            valueLow(LED_AMARELO);
            valueLow(LED_VERMELHO);
        }
        else if (consumo > 25 && consumo <= 50)
        {
            valueLow(LED_VERDE);
            valueHigh(LED_AMARELO);
            valueLow(LED_VERMELHO);
        }
        else if (consumo > 50 && consumo <= 75)
        {
            valueLow(LED_VERDE);
            valueLow(LED_AMARELO);
            valueHigh(LED_VERMELHO);
        }
        else
        { // pegar processo que esta consumindo mais de 75% da cpu e passa pro dontpanic junto com os leds
            if (tipo.compare("CPU") == 0) {
                pid = maiorConsumidor(true);
            } else if (tipo.compare("MEMORY") == 0) {
                pid = maiorConsumidor(false);
            }
            dontpanic(pid, LED_VERDE, LED_AMARELO, LED_VERMELHO, BUTTON);
        }
    }
}
