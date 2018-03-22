#include <iostream>
#include <cstdlib>

void menuSystem();
void stressCPU();
void stressMEMO();

int main()
{
	
	menuSystem();
	return 0;
}


void menuSystem()
{
	short int c = 0;

	std::cout << std::endl;
	std::cout << "1 - Estressar CPU." << std::endl;
	std::cout << "2 - Estressar Memoria." << std::endl;
	std::cout << "0 - Sair." << std::endl;
	std::cin >> c;

	switch(c)
	{
		case 1:
			stressCPU();
		break;
		
		case 2:
//			stressMEMO();
		break;
		
		case 0:
			exit(0);
		break;
	}
}

void stressCPU()
{
	short int c = 0;

	std::cout << std::endl;
	std::cout << "\tO quanto deseja estressar?" << std::endl;
	std::cout << "1 - De 0% a 25%." << std::endl;
	std::cout << "2 - De 25% a 50%." << std::endl;
	std::cout << "3 - de 50% 75%." << std::endl;
	std::cout << "4 - Acima de 75%." << std::endl;
	std::cout << "0 - voltar." << std::endl;
	std::cin >> c;
	
	switch(c)
	{
		case 1:
			system("stress --cpu 10 --timeout 10s");
			menuSystem();
		break;

		case 2:
			system("stress --cpu 300 --timeout 10s");
			menuSystem();
		break;

		case 3:
			system("stress --cpu 1000 --timeout 10s");
			menuSystem();
		break;

		case 4:
			system("stress --cpu 3000 --timeout 10s");
			menuSystem();
		break;

		case 0:
			menuSystem();
		break;
	}	
	
	
}
