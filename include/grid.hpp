#ifndef GRID_HPP
#define GRID_HPP

#include "piece.hpp"
#include <vector>
#include <SFML/Graphics.hpp>

class Grid {
    private:
        int x_offset;
        int y_offset;
        int numrows;
        int numcols;
        std::vector<int> cells;

        int size_cell;
        int line_thickness;

        std::vector<Piece> pieces;

    public:
        void draw(sf::RenderWindow *window);
        //void update(int input);
        //void spawn();
        //void check_lines();
        //void clear_line(int line);

        Grid() {
            x_offset = 10;
            y_offset = 10;
            numrows = 22;
            numcols = 10;
            cells = std::vector<int>(numrows * numcols);

            for (int i = 0; i < numrows; i++) {
                for (int j = 0; j < numcols; j++) {
                    cells[i*numcols+j] = 0;
                }
            }

            size_cell = 40;
            line_thickness = 2;

            pieces = std::vector<Piece>();
        }

        ~Grid() {}
};

#endif