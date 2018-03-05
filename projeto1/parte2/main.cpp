#include <iostream>
#include <cstdlib>
#include <fstream>  
#include <unistd.h>
#include <sys/types.h>
#include <vector>

using namespace std;

//Struct usuários e quantidades de processo
typedef struct {
   
    string userName;
    int numberOfProcess;

} UserAndProcess;



int numberSystemProcess() {
    return system("ps aux | wc -l");
}

void numberSystemProcessByUser() {
    
    system("ps -e -o user > data.txt");
    
    ifstream reader("data.txt", std::ifstream::in);

    string aux;

    vector<UserAndProcess> vectorOfCounting;

    while (reader >> aux) {
        
        if (aux == "USER")
            continue;

        cout <<  aux << endl;

    }



    system("rm data.txt");

}


int main (void) {

    //std::cout << numberSystemProcess() << std::endl;
    numberSystemProcessByUser() ;

    return 0;

}
