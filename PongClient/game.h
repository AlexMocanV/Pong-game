#ifndef GAME_H
#define GAME_H
#include <SFML/Graphics.hpp>
#include <SFML\Network.hpp>
class Game
{
	private:
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
		Game(bool isServer) : isServer(isServer), player1Score(0), player2Score(0), ballSpeedX(0.1f), ballSpeedY(0.1f), windowHeight(600.0f), windowWidth(800.0f), paddleSpeed(0.15f) {}
		void run();
		void loadTextures();
		void setPositions();
		void checkCollision();
		void checkPlayer1Movement(float);
		void checkPlayer2Movement(float);
		void handleClient();
		void handleServer(float);
		void serverSetup();
		void clientSetup();
};
#endif