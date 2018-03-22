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

   
std::string exec(char *cmd);
void menuSystem(int led1, int led2, int led3, int button); //menu de usabilidade
void blinkLed(int led1, int led2, int led3); //piscar todos os leds
void valueHigh(int pin); //setar valor de um pino como high
void valueLow(int pin); //setar valor de um pino como low
bool interruption();
void killProcess(string pid);
bool gpioReady(int pin, string direction);
void panic();
float totalConsumoCPU();
float totalconsumoMEMO();
string maiorConsumidor(bool cpuMemo); //verifica qual é o maior consumidor e retorna seu pid
void monitorarCPU(int led1, int led2, int led3, int button);
void monitorarMemoria(int led1, int led2, int led3, int button);

int main () {

	//Mapeando o pino dos leds
	int pin_led1 = 0; //verde
	int pin_led2 = 0; //amarelo
	int pin_led3 = 0; //vermelho

	//Mapeando o pino do botao
	int pin_button = 0;

	menuSystem(pin_led1, pin_led2, pin_led3, pin_button);
       
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

return true; 
}

void menuSystem(int led1, int led2, int led3, int button)
{
    int c = 0;
    
    std::cout << std::endl; 
    std::cout << "\t Monitoramento" << std::endl;
    std::cout << std::endl;
    std::cout << "1. Monitorar CPU" << std::endl;
    std::cout << "2. Monitorar Memoria" << std::endl;
    std::cout << "0. Sair" << std::endl;    

    std::cin >> c;

    switch(c)
    {
        case 1:
            monitorarCPU(led1, led2, led3, button);
        break;

        case 2:
            monitorarMemoria(led1, led2, led3, button);
        break;
        
        case 0:
            exit(0);
        break; 
    }
}

float totalConsumoCPU()
{

    char * command = "ps -e -o %cpu | sort -r";
    
    string consumoCPU = exec(command);
    std::istringstream reader(consumoCPU);

    float consumoTotal = 0;
   	string line;

    while(std::getline(reader, line))
    {
    	if(line == "%CPU")
    		continue;

    	consumoTotal += atof(line.c_str());
    }


    return consumoTotal;
}

float totalConsumoMEMO()
{

    char * command = "ps -e -o %mem | sort -r";
    
    string consumoMEMO = exec(command);
    std::istringstream reader(consumoMEMO);

    float consumoTotal = 0;
   	string line;

    while(std::getline(reader, line))
    {
    	if(line == "%MEM")
    		continue;

    	consumoTotal += atof(line.c_str());
    }


    return consumoTotal;
}

string maiorConsumidor(bool cpuMemo)
{
	/*cpuMemo
	/ TRUE - retorna o pid do maior consumidor da CPU
	/ FALSE - retorna o pid do maior consumidor da Memoria
	*/

	if(cpuMemo)
	{

	}

}

void monitorarCPU(int led1, int led2, int led3, int button)
{
	std::cout << std::endl;
	std::cout << "\t Monitoramento CPU - Use CTRL C para sair" << std::endl;
	std::cout << ">>Em caso de consumo excessivo utilize o botao do panico." << std::endl;
	std::cout << std::endl;

	float consumo = 0;
	string pid = NULL;
	while(1)
	{
		consumo = totalConsumoCPU();

		if(consumo > 0 && consumo <= 25)
		{
			valueHigh(led1);
			valueLow(led2);
			valueLow(led3);
		}

		else if(consumo > 25 && consumo <= 50)
		{
			valueLow(led1);
			valueHigh(led2);
			valueLow(led3);
		}

		else if(consumo > 50 && consumo <= 75)
		{
			valueLow(led1);
			valueLow(led2);
			valueHigh(led3);			
		}

		else
		{ // pegar processo que esta consumindo mais de 75% da cpu e passa pro panic junto com os leds
			pid = maiorConsumidor(true);
			panic(pid, led1, led2, led3);
		}

	}

	
}
void monitorarMemoria(int led1, int led2, int led3, int button)
{
		std::cout << std::endl;
	std::cout << "\t Monitoramento NEMORIA - Use CTRL C para sair" << std::endl;
	std::cout << ">>Em caso de consumo excessivo utilize o botao do panico." << std::endl;
	std::cout << std::endl;

	float consumo = 0;
	string pid = NULL;
	while(1)
	{
		consumo = totalConsumoMEMO();

		if(consumo > 0 && consumo <= 25)
		{
			valueHigh(led1);
			valueLow(led2);
			valueLow(led3);
		}

		else if(consumo > 25 && consumo <= 50)
		{
			valueLow(led1);
			valueHigh(led2);
			valueLow(led3);
		}

		else if(consumo > 50 && consumo <= 75)
		{
			valueLow(led1);
			valueLow(led2);
			valueHigh(led3);			
		}

		else
		{ // pegar processo que esta consumindo mais de 75% da cpu e passa pro panic junto com os leds
			pid = maiorConsumidor(false);
			panic(pid, led1, led2, led3);
		}
	}
}
