#include "../include/player.hpp"

void Player::display(sf::RenderWindow *window) {
    grid.draw(window);
    // afficher le score et le level
    // afficher les 3 prochaines pieces
}

void Player::update() {
    int input = -1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        input = 0;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        input = 1;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        input = 2;
    } else  {
        input = -1;
    }
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::R) && rotate_clock.getElapsedTime().asMilliseconds() > 300) {
        grid.pieces[0].rotate();
        rotate_clock.restart();
    } 

    if ((input == 0 || input == 1) 
        && update_clock.getElapsedTime().asMilliseconds() > 500) {
        grid.update(input);
        grid.update(2);
        update_clock.restart();
    } else if (input == 2 && update_clock.getElapsedTime().asMilliseconds() > 150) {
        grid.update(2);
        update_clock.restart();
    } else if (update_clock.getElapsedTime().asMilliseconds() > 500) {
        grid.update(2);
        update_clock.restart();
    }

    if (grid.pieces.empty()) {
        grid.spawn(rand()%7);
    }

    std::vector<int> lines = grid.check_lines();
    if (!lines.empty()) {
        for (auto line : lines) {
            grid.clear_line(line);
        }
    }
}