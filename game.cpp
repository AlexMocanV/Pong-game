#include <SFML/Graphics.hpp>
#include "game.h"
#include <iostream>

void Game::loadTextures() {
    if (!player1Texture.loadFromFile("textures/player.png")) {
        std::cerr << "SERVER Failed to load player1 texture\n";
    }
    if (!player2Texture.loadFromFile("textures/player.png")) {
        std::cerr << "SERVER Failed to load player2 texture\n";
    }
    if (!ballTexture.loadFromFile("textures/ball.png")) {
        std::cerr << "SERVER Failed to load ball texture\n";
    }
    player1Sprite.setTexture(player1Texture);
    player2Sprite.setTexture(player2Texture);
    ballSprite.setTexture(ballTexture);

    player1Sprite.setOrigin(player1Sprite.getGlobalBounds().width / 2, player1Sprite.getGlobalBounds().height / 2);
    player1Sprite.setScale(0.5f, 0.5f);
    player2Sprite.setOrigin(player2Sprite.getGlobalBounds().width / 2, player2Sprite.getGlobalBounds().height / 2);
    player2Sprite.setScale(0.5f, 0.5f);
    ballSprite.setOrigin(ballSprite.getGlobalBounds().width / 2, ballSprite.getGlobalBounds().height / 2);
    ballSprite.setScale(0.5f, 0.5f);
}

void Game::setPositions() {
    player1Sprite.setPosition(50.0f, 300.0f); // Player 1 on the left, centered vertically
    player2Sprite.setPosition(750.0f, 300.0f); // Player 2 on the right, centered vertically
    ballSprite.setPosition(400.0f, 300.0f);   // Ball in the center
}

void Game::checkCollision() {
    // Ball collision with paddles
    if (ballSprite.getGlobalBounds().intersects(player1Sprite.getGlobalBounds())) {
        ballSpeedX = std::abs(ballSpeedX) + 0.15f * std::abs(ballSpeedX);
        paddleSpeed = paddleSpeed + 0.15f * paddleSpeed;// Ball moves right after hitting player 1
    }
    if (ballSprite.getGlobalBounds().intersects(player2Sprite.getGlobalBounds())) {
        ballSpeedX = -std::abs(ballSpeedX) - 0.15f * std::abs(ballSpeedX);
        paddleSpeed = paddleSpeed + 0.15f * paddleSpeed;// Ball moves left after hitting player 2
    }
    // Ball out of bounds
    if (ballSprite.getPosition().x < 0.0f) {
        player2Score++;
        ballSprite.setPosition(400.0f, 300.0f); // Reset ball to center
        ballSpeedX = std::abs(initalBallSpeedX); // Start moving right
        paddleSpeed = initialPaddleSpeed;
    }
    if (ballSprite.getPosition().x > 800.0f) {
        player1Score++;
        ballSprite.setPosition(400.0f, 300.0f); // Reset ball to center
        ballSpeedX = -std::abs(initalBallSpeedy);// Start moving left
        paddleSpeed = initialPaddleSpeed;
    }
    // Bounce off top and bottom
    if (ballSprite.getPosition().y < 0.0f || ballSprite.getPosition().y > 600.0f) {
        ballSpeedY = -ballSpeedY;
    }
}

void Game::checkPlayer1Movement(float dt) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        player1Sprite.move(0.0f, -paddleSpeed * dt); // Increased speed
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        player1Sprite.move(0.0f, paddleSpeed * dt); // Increased speed
    }
    if (player1Sprite.getPosition().y < 0.0f) {
        player1Sprite.setPosition(player1Sprite.getPosition().x, 0.0f);
    }
    if (player1Sprite.getPosition().y + player1Sprite.getGlobalBounds().height > 630.0f) {
        player1Sprite.setPosition(player1Sprite.getPosition().x, 630.0f - player1Sprite.getGlobalBounds().height);
    }
}

void Game::checkPlayer2Movement(float dt, int input) {
    if (input == 1) {
        player2Sprite.move(0.0f,  -paddleSpeed * dt); // Increased speed
    }
    if (input == 2) {
        player2Sprite.move(0.0f, paddleSpeed * dt); // Increased speed
    }
    if (player2Sprite.getPosition().y < 0.0f) {
        player2Sprite.setPosition(player2Sprite.getPosition().x, 0.0f);
    }
    if (player2Sprite.getPosition().y + player2Sprite.getGlobalBounds().height > 630.0f) {
        player2Sprite.setPosition(player2Sprite.getPosition().x, 630.0f - player2Sprite.getGlobalBounds().height);
    }
}
void Game::serverSetup()
{
    if (listener.listen(port) != sf::Socket::Done) {
        std::cerr << "Error starting server\n";
        return;
    }
    std::cout << "Server listening on port " << port << '\n';
    if (listener.accept(networkSocket) != sf::Socket::Done) {
        std::cerr << "Error accepting client\n";
        return;
    }
    std::cout << "Client connected\n";
}
void Game::clientSetup()
{
    if (networkSocket.connect(ipAddress, port) != sf::Socket::Done) {
        std::cerr << "Error connecting to server\n";
        return;
    }
    std::cout << "Connected to server\n";
}
void Game::handleServer(float dt)
{
    // Handle local input for Player 1
    checkPlayer1Movement(dt);

    // Receive input from client for Player 2
    sf::Packet packet;
    if (networkSocket.receive(packet) == sf::Socket::Done) {
        int input;
        if (packet >> input) {
            checkPlayer2Movement(dt, input);
        }
    }

    // Update ball
    ballSprite.move(ballSpeedX * dt, ballSpeedY * dt);
    checkCollision();

    // Send game state to client
    sf::Packet statePacket;
    statePacket << player1Sprite.getPosition().y << player2Sprite.getPosition().y
        << ballSprite.getPosition().x << ballSprite.getPosition().y
        << ballSpeedX << ballSpeedY
        << player1Score << player2Score;
    networkSocket.send(statePacket);
}

void Game::handleClient()
{
    int input = 0;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        input = 1;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        input = 2;
    }
    sf::Packet inputPacket;
    inputPacket << input;
    networkSocket.send(inputPacket);

    // Receive game state from server
    sf::Packet statePacket;
    if (networkSocket.receive(statePacket) == sf::Socket::Done) {
        float p1y, p2y, bx, by, bdx, bdy;
        int s1, s2;
        if (statePacket >> p1y >> p2y >> bx >> by >> bdx >> bdy >> s1 >> s2) {
            player1Sprite.setPosition(50.0f, p1y); // x is fixed
            player2Sprite.setPosition(750.0f, p2y); // x is fixed
            ballSprite.setPosition(bx, by);
            ballSpeedX = bdx;
            ballSpeedY = bdy;
            player1Score = s1;
            player2Score = s2;
        }
    }
}

void Game::run() {
    loadTextures();
    setPositions();

    if (isServer) {
        // Server setup
        serverSetup();
    }
    else {
        // Client setup
        clientSetup();
    }
    networkSocket.setBlocking(false); // Non-blocking socket

    sf::RenderWindow window(sf::VideoMode(800, 600), isServer ? "Pong Server" : "Pong Client");
    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        float dt = clock.restart().asSeconds();

        if (isServer) {
            handleServer(dt);
        }
        else {
            // Client: Send input for Player 2
            handleClient();
        }

        sf::Font font;
        if (!font.loadFromFile("fonts/arial.ttf")) {
            std::cerr << "Failed to load font\n";
        }

        sf::Text scoreText;
        scoreText.setFont(font);
        scoreText.setCharacterSize(24);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setPosition(300.0f, 10.0f);
        scoreText.setString("Player 1: " + std::to_string(player1Score) + "  Player 2: " + std::to_string(player2Score));
        // Render
		//move ball and paddles
	    window.clear();
        window.draw(player1Sprite);
        window.draw(player2Sprite);
        window.draw(ballSprite);
        window.draw(scoreText);
        window.display();
    }
}