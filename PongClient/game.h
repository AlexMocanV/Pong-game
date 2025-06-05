#ifndef GAME_H
#define GAME_H
#include <SFML/Graphics.hpp>
#include <SFML\Network.hpp>
class Game
{
private:
	std::string ipAddress;
	int port;
	float initalBallSpeedX = 450.0f;
	float initalBallSpeedy = 450.0f;
	float initialPaddleSpeed = 350.0f;
	bool isServer;
	sf::TcpListener listener;
	sf::TcpSocket networkSocket;
	float windowWidth;
	float windowHeight;
	sf::Sprite player1Sprite;
	sf::Sprite player2Sprite;
	sf::Sprite ballSprite;
	sf::Texture player1Texture;
	sf::Texture player2Texture;
	sf::Texture ballTexture;
	int player1Score;
	int player2Score;
	float ballSpeedX;
	float ballSpeedY;
	float paddleSpeed;
public:
	Game(bool isServer, std::string ip, int port) : ipAddress(ip), port(port), isServer(isServer), player1Score(0), player2Score(0), ballSpeedX(450.0f), ballSpeedY(450.0f), windowHeight(600.0f), windowWidth(800.0f), paddleSpeed(350.0f) {
	}
	void run();
	void loadTextures();
	void setPositions();
	void checkCollision();
	void checkPlayer1Movement(float);
	void checkPlayer2Movement(float dt, int input);
	void handleClient();
	void handleServer(float);
	void serverSetup();
	void clientSetup();
};
#endif