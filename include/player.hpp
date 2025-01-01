#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "grid.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>


class Player {
    private:
        Grid grid;
        int score;
        int level;
        int speed;

    public:
        void handle_input();
        void display();
        void update();
};

#endif