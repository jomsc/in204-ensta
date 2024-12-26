#include "../include/grid.hpp"
#include <iterator>
#include <iostream>

void Grid::draw(sf::RenderWindow *window) {
    // on dessine la grille
    
    // on dessine les lignes horizontales
    for (int i=0;i<=numrows;i++){
        int size = numcols*size_cell+(numcols)*line_thickness;
        sf::RectangleShape ligne(sf::Vector2f(size, line_thickness));
        ligne.setPosition(x_offset, y_offset+i*(size_cell+line_thickness));
        ligne.setFillColor(sf::Color::White);
        window->draw(ligne);
    }

    // on dessine les lignes verticales
    for (int i=0;i<=numcols;i++){
        int size = numrows*size_cell+(numrows+1)*line_thickness;
        sf::RectangleShape ligne(sf::Vector2f(size, line_thickness));
        ligne.setPosition(x_offset+i*(size_cell+line_thickness), y_offset);
        ligne.setFillColor(sf::Color::White);
        ligne.rotate(90);
        window->draw(ligne);
    }

    // on dessine les pieces
    if (!pieces.empty()) {
        for (auto piece : pieces) {
            for (int i=0;i<4;i++) {
                for (int j=0;j<4;j++) {
                    if (piece.shape[i*4+j] == 1) {
                        draw_in_cell(window, piece.x+i, piece.y+j, piece.color);
                    }
                }
            }
        }
    }
}

void Grid::draw_in_cell(sf::RenderWindow *window, int x, int y, sf::Color color) {
    sf::RectangleShape cell(sf::Vector2f(size_cell, size_cell));
    cell.setPosition(x_offset+x*(size_cell+line_thickness), 
        y_offset+y*(size_cell+line_thickness)+line_thickness);
    cell.setFillColor(color);
    window->draw(cell);
}

void Grid::spawn(int type) {
    pieces.insert(pieces.end(), Piece(2, 0, 0, type, 1));
}

void Grid::update(int input) {
    if (!pieces.empty()) {
        for (int i=0;i<pieces.size();i++) {
            switch (input) {
                case 0:
                    // left
                    if (pieces[i].x > 0)
                        pieces[i].x -= 1;
                    break;

                case 1:
                    // right
                    if (pieces[i].x < numcols-4)
                        pieces[i].x += 1;
                    break;

                case 2:
                    // down
                    if (pieces[i].y < numrows-4)
                        pieces[i].y += 1;
                    break;

                default:
                    // default = down
                    if (pieces[i].y < numrows-4)
                        pieces[i].y += 1;
                    break;

            }
        }
    }
}
