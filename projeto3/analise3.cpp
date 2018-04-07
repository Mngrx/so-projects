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

#define BUTTON 115
#define POTENCIOMETRO 5
#define LDR 3


using namespace std;
using namespace chrono;
using namespace this_thread;

std::string exec(const char *cmd);
bool buttonPress(int pin); //retorna se um botão foi pressionado ou não
bool gpioReady(int pin, string direction); //prepara a gpio e muda sua direção
bool gpioFinish(int pin); //desaloca a gpio 
int analogRead(int pin); //leitura da porta analógica
void potenciometroRead(int);
void buttonPressAux(int);
long map(long, long, long, long, long);
void ldrRead(int );

bool potenciometroPreferencia;
bool ldrPreferencia;

bool button_active = false;
int main () {

    gpioReady(BUTTON, "in");

    thread pot(potenciometroRead, POTENCIOMETRO);
    thread but(buttonPressAux, BUTTON);
    thread ldr(ldrRead, LDR);

    pot.detach();
    but.detach();
    ldr.detach();

    while (1) {

        cout << "." << endl;

        sleep_until(system_clock::now() + seconds(2));

    }



}

int analogRead(int pin) {

    string _pin = to_string(pin);

    string comando = "cat /sys/bus/iio/devices/iio:device0/in_voltage" + _pin + "_raw";

    string resultado = exec(comando.c_str());

    istringstream reader(resultado);

    int aux;

    reader >> aux;

    return aux;

}

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void ldrRead(int pin) {

    int valorLido = 0; 
    int valorMapeado;
    int anterior = 0;

    while (1) {
        if(!potenciometroPreferencia)
        {
            valorLido = analogRead(pin);


            if (valorLido >= 3000) {
               ldrPreferencia = false;
            } else {
                ldrPreferencia = true;
                while(1)
                {
                    if(stop_flag == 1)
                    {
                        stop_flag = 0;
                        break;
                    }
                    ptrg->move(DOWN);
                }
            }

            sleep_until(system_clock::now() + seconds(1));
        }
    }

}

void potenciometroRead(int pin) {

    int valorLido = 0; 
    int sentido = 0;
    int valorMapeado;
    int anterior = 0;

    while (1) {

        valorLido = analogRead(pin);

        valorMapeado = map(valorLido, 0, 4095, 0, 255);

        if (anterior == valorMapeado) {
            potenciometroPreferencia = false;
        } else {
            potenciometroPreferencia = true;
            anterior = valorMapeado;
        }

        if(valorMapeado < 80 )
        {
            ptrg->move(LEFT);
        }
        else if(valorMapeado > 160)
        {
            ptrg->move(RIGHT);
        }

        sleep_until(system_clock::now() + seconds(0.5));
    }

}


void buttonPressAux(int pin) {

    while (1) {
        if (!potenciometroPreferencia && !ldrPreferencia) {
             if(buttonPress(pin)) // girar peça
                ptrg->roll();
        }
        sleep_until(system_clock::now() + seconds(0.5));
    }

}



/*@brief Função que executa um comando no terminal e retorna o que seria printao como string.
 *@breaf Creditos: https://stackoverflow.com/questions/32039852/returning-output-from-bash-script-to-calling-c-function.
 *@param cmd - Comando no qual o resultado sera retornado em forma de string.
 */
string exec(const char *cmd)
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

/*
 *@brief Prepara as portas gpio
 *@param pin - Pino no qual sera setado alguma direcao
 *@param direction - Direcao em que se deseja trabalhar com o pino (IN ou OUT).
 *@return bool - TRUE caso o processo ocorra corretamente. FALSE caso o processo falhe.
 */
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

/*
 *@brief Verifica se o button foi pressionado
 *@param pin - Pino o qual o botao esta associado
 *@return bool - TRUE caso o botao esteja acionado. FALSE caso o botao nao esteja acionado.
 */
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