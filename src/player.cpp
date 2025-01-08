#include "player.hpp"

void Player::display(sf::RenderWindow *window) {
    grid.draw(window);
    // afficher le score et le level
    // afficher les 3 prochaines pieces
}


void Player::update_next_pieces(){
            for(int i=0;i<2;i++){
            this->buffer[i]=this->buffer[i+1];
            }
            this->buffer[2]= rand()%7;
        }


void Player::update_score(){
/*à compléter*/
}

void Player::update_level(){
/*à compléter*/
}

void Player::update() {

    int input = -1;
    int input_rota = -1;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        input = 0;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        input = 1;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        input = 2;
    } else  {
        input = -1;
    }
    

    /*if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
        input_rota = 0;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
        input_rota = 1;
    } else  {
        input_rota = -1;
    }*/

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::R) && rotate_clock.getElapsedTime().asMilliseconds() > 200) {
        grid.pieces[0].rotate(0);
        rotate_clock.restart();
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::E) && rotate_clock.getElapsedTime().asMilliseconds() > 200) {
        grid.pieces[0].rotate(1);
        rotate_clock.restart();
    }

    if ((input == 0 || input == 1) 
        && update_clock.getElapsedTime().asMilliseconds() > 155) {
        grid.update(input);
        grid.update(2);
        update_clock.restart();
    } else if (input == 2 && update_clock.getElapsedTime().asMilliseconds() > 155) {
        grid.update(2);
        update_clock.restart();
    } else if (update_clock.getElapsedTime().asMilliseconds() > 500){
        grid.update(2);
        update_clock.restart();
    }

    if (grid.pieces.empty()) {
        grid.spawn(this->buffer[0]);
        update_next_pieces();
    }

    std::vector<int> lines = grid.check_lines();
    if (!lines.empty()) {
        for (auto line : lines) {
            grid.clear_line(line);
        }
    }
}
