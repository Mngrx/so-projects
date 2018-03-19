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

//Struct usuários e quantidades de processo
typedef struct {
   
    string userName;
    int numberOfProcess;

} UserAndProcess;

void menuSystem();
std::string exec(char *cmd);
void numberSystemProcess();
void printNumberSystemProcess();
void printNumberSystemProcessByUser();
void safeForkBomb();
void exportTree();
void printTree();
void identificaForkBob();

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
    FILE *pipe = (cmd, "r");
    if (!pipe)popen
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

void numberSystemProcess() {
    char * command = "ps aux | wc -l";

    short int c = 9;
    int p = 0;
    std::cout << endl << endl;
    std::cout << "======================================================" << endl;
    std::cout << "Escolha a opcao de monitoramento: " << endl;
    while(true)
    {

    	std::cout << "1 - Escolha a quantidade de passos;" << endl;
    	std::cout << "2 - Monitoramento infinito (use CTRL C para encerrar);" << endl;
    	std::cout << "0 - Voltar." << endl;

    	std::cin >> c;
    	switch(c)
    	{
    		case 1:
    			std::cout << "Digite a quantidade de passos: " << endl;
    			std::cin >> p;
    			for(auto i(0); i != p; ++i)
    				std::cout << exec(command);	
    			std::cout << "Monitoramento encerrado..." << endl;
    			numberSystemProcess();
    		break;

    		case 2:
    			while(true)
    			{

    					std::cout << exec(command);	
						sleep_until(system_clock::now() + seconds(3));
    			}	
    		break;

    		case 0:
    		    std::cout << endl;
				std::cout << "======================================================" << endl;
    			menuSystem();

    		break;

    	}
    	
    }
    
}

void printNumberSystemProcess() {
    cout << "Total de processos: ";
    numberSystemProcess();
}

vector<UserAndProcess> numberSystemProcessByUser() {

    char * command = "ps -e -o user | sort | uniq -c";

    string retornoDosUsuarios = exec(command);

    std::istringstream reader(retornoDosUsuarios);

    vector<UserAndProcess> vectorOfCounting;

    int numberOfProcess;
    string user;

    UserAndProcess aux;


    while (reader >> numberOfProcess) {
        
        reader >> user;

        if (user == "USER" && numberOfProcess == 1) 
            continue;

        aux.numberOfProcess = numberOfProcess;
        aux.userName = user;

        vectorOfCounting.push_back(aux);
    }

    return vectorOfCounting;

}

void printNumberSystemProcessByUser() {

vector<UserAndProcess> vectorOfCounting = numberSystemProcessByUser();

 short int c = 9;
    int p = 0;
    std::cout << endl << endl;
    std::cout << "======================================================" << endl;
    std::cout << "Escolha a opcao de monitoramento dos processos por usuario: " << endl;
    while(true)
    {
    	std::cout << "1 - Escolha a quantidade de passos;" << endl;
    	std::cout << "2 - Monitoramento infinito (use CTRL C para encerrar);" << endl;
    	std::cout << "0 - Voltar." << endl;

    	std::cin >> c;
    	switch(c)
    	{
    		case 1:
    			std::cout << "Digite a quantidade de passos: " << endl;
    			std::cin >> p;
    			for(auto i(0); i != p; ++i)
    			{
    				std::cout << "________________" << endl;
    				for (auto i : vectorOfCounting) {
        				std::cout << i.userName << ": " << i.numberOfProcess << endl;
    				}
    			}
    			std::cout << "________________" << endl;
    			std::cout << "Monitoramento encerrado..." << endl;
    			printNumberSystemProcessByUser();
    		break;

    		case 2:
    			while(true)
    			{
    					for (auto i : vectorOfCounting) {
        					std::cout << i.userName << ": " << i.numberOfProcess << endl;
    					}
    					std::cout << "________________" << endl;
    					sleep_until(system_clock::now() + seconds(3));
    			}	
    		break;

    		case 0:
    		    std::cout << endl;
				std::cout << "======================================================" << endl;
    			menuSystem();

    		break;

    	}
    	
    }

}

void printTree()
{
	string pid;
	std::cout << "Insira o PID de onde a arvore deve comecar: ";
	std::cin >> pid;

	char aux[255] = "pstree -A -p ";

	strcat(aux, pid.c_str());

	std::cout << exec(aux);
	std::cout << endl;
	std::cout << "======================================================" << endl;
	menuSystem();

}

void exportTree()
{
	string pid;
	std::cout << "Insira o PID de onde a arvore deve comecar: ";
	std::cin >> pid;
	string comando = "pstree -A -p " + pid + " > arvore" + pid + ".txt";
	system(comando.c_str());
	std::cout << endl;
	std::cout << "======================================================" << endl;
	menuSystem();
}
void printProcessData()
{
	string pid;
	std::cout << "Insira o PID do processo: ";
	std::cin >> pid;
	
	char aux[255] = "ps ";

	strcat(aux, pid.c_str());

	std::cout << exec(aux);

	std::cout << endl;
	std::cout << "======================================================" << endl;
	menuSystem();
}

void safeForkBomb()
{
	string lista, ppid, comando;

    int limit, quantidade;

    std::cout << "Insira o limite de processos: ";
    std::cin >> limit;

    comando = "kill";

   

    while (1) {
        lista = exec("ps -e -o ppid | sort | uniq -c");

        istringstream leitor(lista);

        while (leitor >> quantidade) {
            leitor >> ppid;
            if (quantidade > limit) {
                
				comando = "kill -9 " + ppid;

                cout << comando << endl;

                system(comando.c_str());

                return;
			}
        }
	}
}

void identificaForkBob()
{
	string lista, ppid, comando;

	int limit, quantidade;

	std::cout << "Insira o limite de processos: ";
	std::cin >> limit;

	//comando = "kill";

	while (1)
	{
		lista = exec("ps -e -o ppid | sort | uniq -c");

		istringstream leitor(lista);

		while (leitor >> quantidade)
		{
			leitor >> ppid;
			if (quantidade > limit)
			{

				//comando = "kill -9 " + ppid;

				cout << ppid  << " tá estranho" << endl;

				//system(comando.c_str());

			}
		}
	}
}

void menuSystem()
{
	short int c = 9;
	std::cout << ">>Analise de processos" << endl;
    while(true)
    {
    	std::cout << endl;
        std::cout << "Escolha uma das opcoes abaixo e digite o numero:" << endl << endl;

        std::cout << "1 - Imprimir os dados de um determinado processo;" <<endl;
        std::cout << "2 - Imprimir o numero total de processos no S.O;" << endl;
        std::cout << "3 - Imprimir o numero total de processos no S.O organizados por usuario;" << endl;
        std::cout << "4 - Imprimir uma arvore de hierarquia a partir de um determinado processo;" << endl;
        std::cout << "5 - Exportar a arvore;" << endl;
        std::cout << "6 - Safe Safe Fork Bomb;" << endl;
        std::cout << "7 - Identificador;" << endl;
        std::cout << "0 - Sair." << endl;

        std::cin >> c;

        switch(c)
        {
        	case 0:
        		exit(0);
        	
        	case 1:
        		printProcessData();
        	break;

        	case 2:
        		numberSystemProcess();

        	break;

        	case 3:
        		printNumberSystemProcessByUser();
        	break;

        	case 4:
        		printTree();
        	break;

        	case 5:

        		exportTree();
        	break;

        	case 6:
        		safeForkBomb();
        	break;

			case 7:
				identificaForkBob();
			break;
		}
    }
}


