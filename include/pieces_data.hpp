#ifndef PIECES_DATA_HPP
#define PIECES_DATA_HPP
#include <SFML/Graphics/Color.hpp>
#include <vector>

class PieceData {
    public:
        int offsets[7] = { 0, 16, 48, 80, 112, 176, 240 };
        int shape[304] = { // O, I, S, Z, L, J, T
            // O
            1, 1, 0, 0,
            1, 1, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0,

            // I
            1, 0, 0, 0,
            1, 0, 0, 0,
            1, 0, 0, 0,
            1, 0, 0, 0,
            
            1, 1, 1, 1,
            0, 0, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0,
            
            // S
            0, 1, 1, 0,
            1, 1, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0,

            1, 0, 0, 0,
            1, 1, 0, 0,
            0, 1, 0, 0,
            0, 0, 0, 0,

            // Z
            1, 1, 0, 0,
            0, 1, 1, 0,
            0, 0, 0, 0,
            0, 0, 0, 0,

            0, 1, 0, 0,
            1, 1, 0, 0,
            1, 0, 0, 0,
            0, 0, 0, 0,

            // L
            1, 1, 1, 0,
            1, 0, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0,

            1, 1, 0, 0,
            0, 1, 0, 0,
            0, 1, 0, 0,
            0, 0, 0, 0,

            0, 0, 1, 0,
            1, 1, 1, 0,
            0, 0, 0, 0,
            0, 0, 0, 0,

            1, 0, 0, 0,
            1, 0, 0, 0,
            1, 1, 0, 0,
            0, 0, 0, 0,

            // J
            1, 1, 1, 0,
            0, 0, 1, 0,
            0, 0, 0, 0,
            0, 0, 0, 0,

            0, 1, 0, 0,
            0, 1, 0, 0,
            1, 1, 0, 0,
            0, 0, 0, 0,

            1, 0, 0, 0,
            1, 1, 1, 0,
            0, 0, 0, 0,
            0, 0, 0, 0,

            1, 1, 0, 0,
            1, 0, 0, 0,
            1, 0, 0, 0,
            0, 0, 0, 0,

            // T
            1, 1, 1, 0,
            0, 1, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0,

            0, 1, 0, 0,
            1, 1, 0, 0,
            0, 1, 0, 0,
            0, 0, 0, 0,

            0, 1, 0, 0,
            1, 1, 1, 0,
            0, 0, 0, 0,
            0, 0, 0, 0,

            1, 0, 0, 0,
            1, 1, 0, 0,
            1, 0, 0, 0,
            0, 0, 0, 0
        };
        std::vector<sf::Color> colors = {
            sf::Color::White,
            sf::Color::Green,
            sf::Color::Blue,
            sf::Color::Magenta,
            sf::Color::Red,
            sf::Color::Cyan,
            sf::Color::Yellow
        };

        ~PieceData() {}
};

#endif 