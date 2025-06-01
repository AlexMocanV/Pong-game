#include <SFML/Graphics.hpp>
#include "game.h"
#include <iostream>

void Game::loadTextures() {
    if (!player1Texture.loadFromFile("textures/player.png")) {
        std::cerr << "Failed to load player1 texture\n";
    }
    if (!player2Texture.loadFromFile("textures/player.png")) {
        std::cerr << "Failed to load player2 texture\n";
    }
    if (!ballTexture.loadFromFile("textures/ball.png")) {
        std::cerr << "Failed to load ball texture\n";
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
        ballSpeedX = std::abs(ballSpeedX); // Ball moves right after hitting player 1
    }
    if (ballSprite.getGlobalBounds().intersects(player2Sprite.getGlobalBounds())) {
        ballSpeedX = -std::abs(ballSpeedX); // Ball moves left after hitting player 2
    }
    // Ball out of bounds
    if (ballSprite.getPosition().x < 0.0f) {
        player2Score++;
        ballSprite.setPosition(400.0f, 300.0f); // Reset ball to center
        ballSpeedX = std::abs(ballSpeedX); // Start moving right
    }
    if (ballSprite.getPosition().x > 800.0f) {
        player1Score++;
        ballSprite.setPosition(400.0f, 300.0f); // Reset ball to center
        ballSpeedX = -std::abs(ballSpeedX); // Start moving left
    }
    // Bounce off top and bottom
    if (ballSprite.getPosition().y < 0.0f || ballSprite.getPosition().y > 600.0f) {
        ballSpeedY = -ballSpeedY;
    }
}

void Game::checkPlayer1Movement() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        player1Sprite.move(0.0f, -0.15f); // Increased speed
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        player1Sprite.move(0.0f, 0.15f); // Increased speed
    }
    if (player1Sprite.getPosition().y < 0.0f) {
        player1Sprite.setPosition(player1Sprite.getPosition().x, 0.0f);
    }
    if (player1Sprite.getPosition().y + player1Sprite.getGlobalBounds().height > 630.0f) {
        player1Sprite.setPosition(player1Sprite.getPosition().x, 630.0f - player1Sprite.getGlobalBounds().height);
    }
}

void Game::checkPlayer2Movement() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        player2Sprite.move(0.0f, -0.15f); // Increased speed
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        player2Sprite.move(0.0f, 0.15f); // Increased speed
    }
    if (player2Sprite.getPosition().y < 0.0f) {
        player2Sprite.setPosition(player2Sprite.getPosition().x, 0.0f);
    }
    if (player2Sprite.getPosition().y + player2Sprite.getGlobalBounds().height > 630.0f) {
        player2Sprite.setPosition(player2Sprite.getPosition().x, 630.0f - player2Sprite.getGlobalBounds().height);
    }
}

void Game::run() {
    // Load textures
    loadTextures();
    setPositions();
    // Load font for score display
    sf::Font font;
    if (!font.loadFromFile("fonts/arial.ttf")) {
        std::cerr << "Failed to load font\n";
    }
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(300.0f, 10.0f);

    // Main game loop
    sf::RenderWindow window(sf::VideoMode(800, 600), "Pong Game");
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        // Handle player input
        checkPlayer1Movement();
        checkPlayer2Movement();
        // Check for collisions
        checkCollision();
        ballSprite.move(ballSpeedX, ballSpeedY);
        // Update score display
        scoreText.setString("Player 1: " + std::to_string(static_cast<int>(player1Score)) +
            "  Player 2: " + std::to_string(static_cast<int>(player2Score)));
        window.clear();
        window.draw(player1Sprite);
        window.draw(player2Sprite);
        window.draw(ballSprite);
        window.draw(scoreText);
        window.display();
    }
}