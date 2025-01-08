#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "grid.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <cstdlib>


class Player {
    private:
        Grid grid;
        int score;
        int level;
        int speed;
        int buffer[3]; /*pour les 3 prochaines pièces*/

        sf::Clock update_clock;
        sf::Clock rotate_clock;

    public:

        void display(sf::RenderWindow *window);
        void update();
        void update_next_pieces();
        void update_score();
        void update_level();


        Player() {
            grid = Grid();
            score = 0;
            level = 1;
            speed = 1;
            for(int i=0;i<=2;i++){
                buffer[i]=rand()%7;
            }
            update_clock.restart();
            rotate_clock.restart();

            grid.spawn(rand()%7);
        }
};

#endif