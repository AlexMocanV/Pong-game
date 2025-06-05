#include <iostream>
#include "game.h"
#include <SFML\Network.hpp>
int main()
{
	std::cout << "Enter server ip:\n" << std::endl;
	std::string ip;
	std::getline(std::cin, ip);
	std::cout << "Enter ip port:\n" << std::endl; 
	int port = 0;
	std::cin >> port;
	Game game(false, ip, port);
	game.run();

	return 0;
}