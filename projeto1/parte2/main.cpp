#include <iostream>
#include <cstdlib>
#include <sstream>  
#include <unistd.h>
#include <sys/types.h>
#include <vector>

using namespace std;

//Struct usuários e quantidades de processo
typedef struct {
   
    string userName;
    int numberOfProcess;

} UserAndProcess;

// Função que executa um comando no terminal e retorna o que seria printado como string
// Creditos: https://stackoverflow.com/questions/32039852/returning-output-from-bash-script-to-calling-c-function
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

string numberSystemProcess() {
    char * command = "ps aux | wc -l";
    return exec(command);
}

void printNumberSystemProcess() {
    cout << "Total de processos: " << numberSystemProcess();
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

    cout << "Processos por usuário:\n";

    for (auto i : vectorOfCounting) {
        cout << i.userName << ": " << i.numberOfProcess << endl;
    }

}


int main (int argc, char * argv[]) {

    /* Argumentos para passar na hora da execução
    *   -u : quantidade de processos por usuário [0]
    *   -t : quantidade de processos totais (padrão) [1]
    *   -m : modo de monitoramento [0]
    *   -o : imprime os dados do instante em que foi executado (padrão) [1]
    */
     
    int modoMonitoramento = modoImpressao = 1;

    string input;

    if (argc > 1) {
        for (int i = 1; i < argc; ++i) {
            if (argv[i] == "-u") {
                modoImpressao = 0;
            }
            if (argv[i] == "-t") {
                modoImpressao = 1;
            }
            if (argv[i] == "-m") {
                modoMonitoramento = 0;
            }
            if (argv[i] == "-o") {
                modoMonitoramento = 1;
            }
        }
    }

    for () {
        
    }

    return 0;

}
