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

        int level_up_buffer;
        int num_lines_cleared;

        sf::Clock update_clock;
        sf::Clock rotate_clock;

    public:

        void display(sf::RenderWindow *window);
        void update();
        void update_next_pieces();
        void update_score(int num_lines_cleared,int level);
        void update_level();


        Player() {
            grid = Grid();
            score = 0;
            level = 0;
            speed = 1;
            for(int i=0;i<=2;i++){
                buffer[i]=rand()%7;
            }

            level_up_buffer=0;
            num_lines_cleared=0;
            update_clock.restart();
            rotate_clock.restart();

            grid.spawn(rand()%7);
        }
};

#endif