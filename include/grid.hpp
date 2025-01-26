#ifndef GRID_HPP
#define GRID_HPP

#include "piece.hpp"
#include <SFML/Graphics.hpp>

class Grid {
    private:
        int x_offset;
        int y_offset;
        int numrows;
        int numcols;
        int size_cell;
        int line_thickness;
        sf::Font arial;

        

    public:
        std::vector<int> cells; 
        // grounded piece : type+1, falling piece : 0, empty : -1
        std::vector<Piece> pieces; // pieces qui tombent

        void draw(sf::RenderWindow *window, int score, int level);
        void draw_in_cell(sf::RenderWindow *window, int x, int y, sf::Color color);
        void spawn(int type);
        void update(bool* floor, bool* left_wall, bool* right_wall, bool* lock_in);
        void top_out();

        bool impossible_kick(int x1,int y1,int orientation);
        void wall_kick_clockwise(int type, int orientation);
        void wall_kick_counterclockwise(int type, int orientation);

        std::vector<int> check_lines();
        void clear_line(int line);
        int get_dimensions() { return numrows*numcols; }
        bool hasLost() {}

        Grid() {
            x_offset = 10;
            y_offset = 10;
            numrows = 22;
            numcols = 10;
            cells = std::vector<int>(numrows * numcols);

            for (int i = 0; i < numrows; i++) {
                for (int j = 0; j < numcols; j++) {
                    cells[i*numcols+j] = -1;
                }
            }

            size_cell = 40;
            line_thickness = 1;

            pieces = std::vector<Piece>();

            arial.loadFromFile("./assets/fonts/arial.ttf");
        }

        ~Grid() {}
};

#endif