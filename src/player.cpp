#include "player.hpp"
#include <iostream>

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

void Player::update_score(int num_lines_cleared,int level){
    switch (num_lines_cleared)
    {
    case 1:
        this->score += level*40;
        this->num_lines_cleared=0;
        break;
    
    case 2:
        this->score += level*100;
        this->num_lines_cleared=0;
        break;

    case 3:
        this->score += level*300;
        this->num_lines_cleared=0;
        break;

    case 4:
        this->score += level*1200;
        this->num_lines_cleared=0;
        break;
    
    default:
        break;
    }
}

void Player::update_level(){
    if(this->level_up_buffer>=10){
        this->level++;
        this->level_up_buffer=this->level_up_buffer%10;
    }
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

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::R) && 
            rotate_clock.getElapsedTime().asMilliseconds() > 200) {
        grid.pieces[0].rotate(0);
        rotate_clock.restart();
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::E) && 
            rotate_clock.getElapsedTime().asMilliseconds() > 200) {
        grid.pieces[0].rotate(1);
        rotate_clock.restart();
    }
    
    float speed = get_speed(this->level-1);
    std::cout << "Speed :"<< speed << std::endl;
    if (gravity_clock.getElapsedTime().asSeconds() > speed) {
        grid.update(2);
        gravity_clock.restart();
    }

    if (movement_clock.getElapsedTime().asMilliseconds() > 155) {
        grid.update(input);
        movement_clock.restart();
    } 

    if (grid.pieces.empty()) {
        grid.spawn(this->buffer[0]);
        update_next_pieces();
    }


    std::vector<int> lines = grid.check_lines();
    if (!lines.empty()) {
        this->num_lines_cleared =0;
        for (auto line : lines) {
            this->num_lines_cleared++;
            this->level_up_buffer++;
            grid.clear_line(line);
        }
    }

    update_score(this->num_lines_cleared,this->level);
    std::cout << "Score :"<<this->score << std::endl; 

    update_level();
    std::cout << "Level :"<<this->level << std::endl; 
}
