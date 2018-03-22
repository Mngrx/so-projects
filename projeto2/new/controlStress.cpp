#include <iostream>
#include <cstdlib>

void menuSystem();
void stressControlCPU();
void stressControlMEMO();

int main()
{
	
	menuSystem();
	return 0;
}


void menuSystem()
{
	short int c = 0;

	std::cout << std::endl;
	std::cout << "1 - Controlar % da CPU do processo stress." << std::endl;
	std::cout << "2 - Estressar Memoria." << std::endl;
	std::cout << "0 - Sair." << std::endl;
	std::cin >> c;

	switch(c)
	{
		case 1:
			stressControlCPU();
		break;
		
		case 2:
			stressControlMEMO();
		break;
		
		case 0:
			exit(0);
		break;
	}
}

void stressControlCPU()
{
	short int c = 0;

	std::cout << std::endl;
	std::cout << "\tO quanto deseja controlar?" << std::endl;
	std::cout << "1 - De 0% a 25%." << std::endl;
	std::cout << "2 - De 25% a 50%." << std::endl;
	std::cout << "3 - de 50% 75%." << std::endl;
	std::cout << "4 - Acima de 75%." << std::endl;
	std::cin >> c;
	
	switch(c)
	{
		case 1:
			system("cpulimit -e stress -l 10");
		break;

		case 2:
			system("cpulimit -e stress -l 25");
		break;

		case 3:
			system("cpulimit -e stress -l 50");
		break;

		case 4:
			system("cpulimit -e stress -l 75");
		break;

	}	
	
	
}

void stressControlMEMO() {

	short int c = 0;

	std::cout << std::endl;
	std::cout << "\tO quanto deseja controlar?" << std::endl;
	std::cout << "1 - De 0% a 25%." << std::endl;
	std::cout << "2 - De 25% a 50%." << std::endl;
	std::cout << "3 - de 50% 75%." << std::endl;
	std::cout << "4 - Acima de 75%." << std::endl;
	std::cin >> c;
	
	switch(c)
	{
		case 1:
			system("stress --vm 1 --vm-bytes 64M");
		break;

		case 2:
			system("stress --vm 1 --vm-bytes 128M");
		break;

		case 3:
			system("stress --vm 1 --vm-bytes 196M");
		break;

		case 4:
			system("stress --vm 1 --vm-bytes 300M");
		break;

	}	
	


}

