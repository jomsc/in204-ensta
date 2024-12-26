#ifndef PIECE_HPP
#define PIECE_HPP
#include <SFML/Graphics/Color.hpp>
#include "pieces_data.hpp"
#include <iostream>

class Piece {
    public:
        int x;
        int y;
        int orientation;
        int type; 
        int value;
        int shape[16]; // 4x4 array pour decrire le tetrimino
        sf::Color color;
        bool is_grounded;

        Piece() {
            this->x = 0;
            this->y = 0;
            this->orientation = 0;
            this->type = 4;
            this->value = 1;

            PieceData piece_data = PieceData();
            for (int i = 0; i < 16; i++) {
                this->shape[i+112] = piece_data.shape[i+112];
            }
            piece_data.~PieceData();

            this->color = sf::Color::Red;
            this->is_grounded = false;
        }

        Piece(int x, int y, int orientation, int type, int value) {
            this->x = x;
            this->y = y;
            this->orientation = orientation;
            this->type = type;
            this->value = value;

            PieceData piece_data = PieceData();

            for (int i = 0; i < 16; i++) {
                this->shape[i] = piece_data.shape[i+piece_data.offsets[type]];
            }
            piece_data.~PieceData();
            this->is_grounded = false;

            switch (type) {
                case 0:
                    this->color = sf::Color::White;
                    break;
                case 1:
                    this->color = sf::Color::Green;
                    break;
                case 2:
                    this->color = sf::Color::Blue;
                    break;
                case 3:
                    this->color = sf::Color::Magenta;
                    break;
                case 4:
                    this->color = sf::Color::Red;
                    break;
                case 5:
                    this->color = sf::Color::Cyan;
                    break;
                case 6:
                    this->color = sf::Color::Yellow;
                    break;
                default:
                    this->color = sf::Color::Red;
                    break;
            }
        }

        ~Piece() {}

        void rotate() {
            switch (this->type) {
                case 0:
                    return;
                case 1:
                    this->orientation = !this->orientation;
                    break;
                case 2:
                    this->orientation = !this->orientation;
                    break;
                case 3:
                    this->orientation = !this->orientation;
                    break;
                case 4:
                    this->orientation = (this->orientation+1) % 4;
                    break;
                case 5:
                    this->orientation = (this->orientation+1) % 4;
                    break;
                case 6:
                    this->orientation = (this->orientation+1) % 4;
                    break;
                default:
                    return;

            }

            PieceData piece_data = PieceData();

            for (int i = 0; i < 16; i++) {
                this->shape[i] = piece_data.shape[i+piece_data.offsets[this->type]+this->orientation*16];
            }
            piece_data.~PieceData();
        }
};

#endif