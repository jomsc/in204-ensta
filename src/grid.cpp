#include "../include/grid.hpp"

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
    
}