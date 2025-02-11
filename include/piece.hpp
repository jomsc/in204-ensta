#ifndef PIECE_HPP
#define PIECE_HPP

#include <SFML/Graphics/Color.hpp>
#include "pieces_data.hpp"

class Piece {
    public:
        int x;
        int y;
        int orientation;
        int type; 
        int value;
        int shape[16]; // 4x4 array pour decrire le tetrimino
        sf::Color color;

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
            
            this->color = piece_data.colors[type];
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
            this->color = piece_data.colors[type];
        }

        ~Piece() {}

        void rotate(int input_rota) {
            switch (this->type) {
                case 0:
                    return;
                case 1:
                    if(input_rota==0)
                    {
                        this->orientation = (this->orientation+1) % 4;
                        break;
                    } else 
                    {
                        this->orientation = (this->orientation+3) % 4;
                        break;
                    }
                case 2:
                    if(input_rota==0)
                    {
                        this->orientation = (this->orientation+1) % 4;
                        break;
                    } else 
                    {
                        this->orientation = (this->orientation+3) % 4;
                        break;
                    }
                case 3:
                    if(input_rota==0)
                    {
                        this->orientation = (this->orientation+1) % 4;
                        break;
                    } else 
                    {
                        this->orientation = (this->orientation+3) % 4;
                        break;
                    }
                case 4:
                    if(input_rota==0)
                    {
                        this->orientation = (this->orientation+1) % 4;
                        break;
                    } else 
                    {
                        this->orientation = (this->orientation+3) % 4;
                        break;
                    }
                case 5:
                      if(input_rota==0)
                    {
                        this->orientation = (this->orientation+1) % 4;
                        break;
                    } else 
                    {
                        this->orientation = (this->orientation+3) % 4;
                        break;
                    }
                case 6:
                      if(input_rota==0)
                    {
                        this->orientation = (this->orientation+1) % 4;
                        break;
                    } else 
                    {
                        this->orientation = (this->orientation+3) % 4;
                        break;
                    }
                default:
                    return;
            }

            PieceData piece_data = PieceData();
            for (int i = 0; i < 16; i++) {
                this->shape[i] = piece_data.shape[i+piece_data.offsets[this->type]+this->orientation*16];
            }
            //piece_data.~PieceData();
        }

        void move(int move_input)
        {
            switch(move_input)
            {
                case 0: //gauche
                    this->x-=1;
                    break;
                case 1: //droite
                    this->x+=1;
                    break;
                case 2: //bas
                    this->y+=1;
                    break;
                case 3: //haut pour les wall_kick
                    this->y-=1;
                    break;
                case -1:
                    break;
            }
        }
};



#endif