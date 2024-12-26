#ifndef PIECE_HPP
#define PIECE_HPP
#include <SFML/Graphics/Color.hpp>

class Piece {
    public:
        int x;
        int y;
        int orientation;
        int type; // O, I, S, Z, L, J, T
        int value;
        int shape[8]; // 4x2 array pour decrire le tetrimino
        sf::Color color;

        Piece() {
            x = 0;
            y = 0;
            orientation = 0;
            type = 0;
            value = 1;

            // default shape = L
            shape[0] = 0;
            shape[1] = 0;
            shape[2] = 1;
            shape[3] = 0;
            shape[4] = 1;
            shape[5] = 0;
            shape[6] = 1;
            shape[7] = 1;

            // default color = red
            color = sf::Color::Red;
        }

        Piece(int x, int y, int orientation, int type, int value) {
            this->x = x;
            this->y = y;
            this->orientation = orientation;
            this->type = type;
            this->value = value;

            switch (type) { // O, I, S, Z, L, J, T
                case 0:
                    // O
                    shape[0] = 0;
                    shape[1] = 0;
                    shape[2] = 0;
                    shape[3] = 0;
                    shape[4] = 1;
                    shape[5] = 1;
                    shape[6] = 1;
                    shape[7] = 1;
                    this->color = sf::Color::White;
                    break;
                
                case 1:
                    // I
                    shape[0] = 1;
                    shape[1] = 0;
                    shape[2] = 1;
                    shape[3] = 0;
                    shape[4] = 1;
                    shape[5] = 0;
                    shape[6] = 1;
                    shape[7] = 0;
                    this->color = sf::Color::Green;
                    break;

                case 2:
                    // S
                    shape[0] = 0;
                    shape[1] = 0;
                    shape[2] = 1;
                    shape[3] = 0;
                    shape[4] = 1;
                    shape[5] = 1;
                    shape[6] = 0;
                    shape[7] = 1;
                    this->color = sf::Color::Blue;
                    break;

                case 3:
                    // Z
                    shape[0] = 0;
                    shape[1] = 0;
                    shape[2] = 0;
                    shape[3] = 1;
                    shape[4] = 1;
                    shape[5] = 1;
                    shape[6] = 1;
                    shape[7] = 0;
                    this->color = sf::Color::Yellow;
                    break;
                
                case 4:
                    // L
                    shape[0] = 0;
                    shape[1] = 0;
                    shape[2] = 1;
                    shape[3] = 0;
                    shape[4] = 1;
                    shape[5] = 0;
                    shape[6] = 1;
                    shape[7] = 1;
                    this->color = sf::Color::Red;
                    break;
                
                case 5:
                    // J
                    shape[0] = 0;
                    shape[1] = 0;
                    shape[2] = 0;
                    shape[3] = 1;
                    shape[4] = 0;
                    shape[5] = 1;
                    shape[6] = 1;
                    shape[7] = 1;
                    this->color = sf::Color::Magenta;
                    break;
                
                case 6:
                    // T
                    shape[0] = 0;
                    shape[1] = 0;
                    shape[2] = 1;
                    shape[3] = 0;
                    shape[4] = 1;
                    shape[5] = 1;
                    shape[6] = 1;
                    shape[7] = 0;
                    this->color = sf::Color::Cyan;
                    break;

                default: 
                    // L
                    shape[0] = 0;
                    shape[1] = 0;
                    shape[2] = 1;
                    shape[3] = 0;
                    shape[4] = 1;
                    shape[5] = 0;
                    shape[6] = 1;
                    shape[7] = 1;
                    this->color = sf::Color::Red;
                    break;
            }
        }

        ~Piece() {}
};

#endif