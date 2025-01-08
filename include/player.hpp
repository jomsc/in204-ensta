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
        int buffer[3]; /*pour les 3 prochaines pièces*/

        int level_up_buffer;
        int num_lines_cleared;

        sf::Clock gravity_clock;
        sf::Clock rotate_clock;
        sf::Clock movement_clock;

        float speeds[15] = { // cells per frame at 60fps (official guidelines)
            0.01667, 
            0.021017, 
            0.026977, 
            0.035256,
            0.04693,
            0.06361,
            0.0879,
            0.1236,
            0.1775,
            0.2598,
            0.388,
            0.59,
            0.92,
            1.46,
            2.36
        };

    public:

        void display(sf::RenderWindow *window);
        void update();
        void update_next_pieces();
        void update_score(int num_lines_cleared,int level);
        void update_level();


        Player() {
            grid = Grid();
            score = 0;
            level = 1;
            for(int i=0;i<=2;i++){
                buffer[i]=rand()%7;
            }

            level_up_buffer=0;
            num_lines_cleared=0;
            gravity_clock.restart();
            rotate_clock.restart();
            movement_clock.restart();

            grid.spawn(rand()%7);
        }

        float get_speed(int level) {
            int i = (level > 15) ? 15 : level; // vitesse max = niveau 15
            return 1/(60*speeds[i]);
        }
};

#endif