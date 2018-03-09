#include <iostream>
#include <cstdlib>
#include <sstream>
#include <unistd.h>
#include <sys/types.h>

using namespace std;

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

int main(int argc, char *argv[]) {

    string lista, ppid, comando;
    int limit = 200, quantidade, i = 0;

    //cin >> limit;

    comando = "kill";

   

    while (1) {
        lista = exec("ps -e -o ppid | sort | uniq -c");

        istringstream leitor(lista);

        while (leitor >> quantidade) {
            leitor >> ppid;
            //cout << ppid << ": "<< quantidade << endl;
            //cout << ".";
            if (quantidade > limit) {
                comando = "kill -9 " + ppid;

                cout << comando << endl;

                system(comando.c_str());

                return 0;

            }
        }
    }


}


