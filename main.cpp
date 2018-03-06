#include <iostream>
#include <cstdlib>
#include <fstream>  
#include <unistd.h>
#include <sys/types.h>
#include <vector>
#include <string>

using namespace std;

//Struct usuários e quantidades de processo
typedef struct {
   
    string userName;
    int numberOfProcess = 0;

}userProcess; 


int numberSystemProcess2() {
    return system("ps aux | wc -l");
}


void printProcessByUser(std::vector<string> process,
std::vector<userProcess> &processByUser)
{
    int c = 0;
    
    if(process.size() != 0)
    {
        userProcess firstProcess;
        firstProcess.userName = *(process.begin());
        firstProcess.numberOfProcess++;
        processByUser.push_back(firstProcess);
    }

//AQUI CONTAVA E SEPARAVA CADA PROCESSO PARA CADA USUARIO, MAS ESTOU ERRANDO,
//POIS TESTA PARA CADA ITERAÇÃO DO FOR. SEI CONSERTAR, MAS ESTOU COM PREGUIÇA
//VOU FAZER UMA COISA ANTES...    
/*
    for(auto it = process.begin()+1; it !=
process.end(); ++it)
    {
        for(auto it2 = processByUser.begin(); it2 != processByUser.end(); ++it2)
        {
            if(*it.compare((*it2).userName) != 0)
            {
               userProcess otherProcess;
               otherProcess.userName = *it2;
               otherProcess.numberOfProcess++;
               processByUser.push_back(otherProcess);
            }

            else
               *(it2).numberOfProcess++;
        } 
     }
  */  
  
    
}

void systemProcess(std::vector<string> &process)
{
    system("ps -e -o user > data.txt");
    ifstream reader("data.txt", std::ifstream::in);
    string aux;

    while (reader >> aux) {
        
        if (aux == "USER")
            continue;

        process.push_back(aux);

    }
    
    std::cout << process.size();
    system("rm data.txt");

}


int main (void) {
    
    std::vector<string> process;   
    std::vector<userProcess> processByUser;
   
   
   cout << endl;
   numberSystemProcess2();
   cout << endl;
   systemProcess(process);
   cout << endl;
   numberSystemProcess2();
    cout << endl;
    printProcessByUser(process, processByUser);
    cout << process.size();
   return 0;

}
