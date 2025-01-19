#include "grid.hpp"
#include <iterator>

void Grid::draw(sf::RenderWindow *window, int score, int level) {
    // on dessine la grille
   
    // on dessine les lignes horizontales
    for (int i=0;i<=numrows;i++) {
        int size = numcols*size_cell+(numcols)*line_thickness;
        sf::RectangleShape ligne(sf::Vector2f(size, line_thickness));
        ligne.setPosition(x_offset, y_offset+i*(size_cell+line_thickness));
        ligne.setFillColor(sf::Color::White);
        window->draw(ligne);
    }

    // on dessine les lignes verticales
    for (int i=0;i<=numcols;i++) {
        int size = numrows*size_cell+(numrows+1)*line_thickness;
        sf::RectangleShape ligne(sf::Vector2f(size, line_thickness));
        ligne.setPosition(x_offset+i*(size_cell+line_thickness), y_offset);
        ligne.setFillColor(sf::Color::White);
        ligne.rotate(90);
        window->draw(ligne);
    }
    // on dessine les cells occupées
    PieceData piece_data = PieceData();
    for (int i=0;i<numrows;i++) {
        for (int j=0;j<numcols;j++){
            if (cells[i*numcols+j]>0) {
                sf::Color color = piece_data.colors[cells[i*numcols+j]-1];
                color.r = std::max(0, color.r - 60);
                color.g = std::max(0, color.g - 60);
                color.b = std::max(0, color.b - 60);
                draw_in_cell(window, j, i, color);
            }
        }
    }
    //piece_data.~PieceData();

    // on dessine les pieces qui tombent
    if (!pieces.empty()) {
        for (auto piece : pieces) {
            for (int i=0;i<4;i++) {
                for (int j=0;j<4;j++) {
                    if (piece.shape[i*4+j] == 1) {
                        draw_in_cell(window, piece.x+j, piece.y+i, piece.color);
                    }
                }
            }
        }
    }

    // on dessine le score
    std::string scorestr = "Score : " + std::to_string(score);
    sf::Text score_text(scorestr, arial);
    score_text.setCharacterSize(30);
    score_text.setStyle(sf::Text::Bold);
    score_text.setFillColor(sf::Color::White);
    score_text.setPosition(numcols*(size_cell+line_thickness)
                           +line_thickness+x_offset*2, y_offset);
    window->draw(score_text);

    // on dessine le niveau 
    std::string levelstr = "Niveau : " + std::to_string(level);
    sf::Text level_text(levelstr, arial);
    level_text.setCharacterSize(30);
    level_text.setStyle(sf::Text::Bold);
    level_text.setFillColor(sf::Color::White);
    level_text.setPosition(numcols*(size_cell+line_thickness)
                           +line_thickness+x_offset*2, 2*y_offset+30);
    window->draw(level_text);
}

void Grid::draw_in_cell(sf::RenderWindow *window, int x, int y, sf::Color color) {
    sf::RectangleShape cell(sf::Vector2f(size_cell, size_cell));
    cell.setPosition(x_offset+x*(size_cell+line_thickness), 
        y_offset+y*(size_cell+line_thickness)+line_thickness);
    cell.setFillColor(color);
    window->draw(cell);
}

void Grid::spawn(int type) {
    pieces.emplace_back(2, 0, 0, type, 1);

    for (int i=0;i<4;i++) {
        for (int j=0;j<4;j++) {
            if (pieces[pieces.size()-1].shape[4*i+j] == 1) {
                this->cells[(pieces[pieces.size()-1].y+i)*numcols+pieces[pieces.size()-1].x+j] = 0;
            }
        }
    }
}

void Grid::update(int input) {

    if (!pieces.empty()) {
        for (int v=0;v<pieces.size();v++) {
            // on enleve la piece de la grid
            for (int i=0;i<4;i++) {
                for (int j=0;j<4;j++) {
                    if (pieces[v].x+j >= 0 
                        && pieces[v].x+j < numcols 
                        && pieces[v].y+i >= 0 
                        && pieces[v].y+i < numrows) {
                        
                        if (pieces[v].shape[4*i+j] == 1) {
                            this->cells[(pieces[v].y+i)*numcols + pieces[v].x+j] = -1; 
                        }
                    }
                }

            }

            bool possible = true;
            // on bouge la pièce en fonction de l'input
            switch (input) {
                case 0: 
                    // gauche
                    
                    for (int i=0;i<4;i++) {
                        for (int j=0;j<4;j++) {
                            if (pieces[v].shape[4*i+j] == 1) {
                                if (pieces[v].x+j < 1 ||
                                    cells[(pieces[v].y+i)*numcols + pieces[v].x+j-1] > 0) {
                                    possible = false;
                                }
                            }
                        }
                    }
                    if (possible)
                        pieces[v].x -= 1;
                        possible = false;
                    break;

                case 1: 
                    // droite
                    for (int i=0;i<4;i++) {
                        for (int j=0;j<4;j++) {
                            if (pieces[v].shape[4*i+j] == 1) {
                                if (pieces[v].x+j >= numcols-1 ||
                                    cells[(pieces[v].y+i)*numcols + pieces[v].x+j+1] > 0) {
                                    possible = false;
                                }
                            }
                        }
                    }
                    if (possible)
                        pieces[v].x += 1;
                        possible = false;
                    break;

                case 2: 
                    // bas
                    for (int i=0;i<4;i++) {
                        for (int j=0;j<4;j++) {
                            if (pieces[v].shape[4*i+j] == 1) {
                                if (pieces[v].y+i >= numrows-1 ||
                                    cells[(pieces[v].y+i+1)*numcols + pieces[v].x+j] > 0) {
                                    possible = false;
                                }
                            }
                        }
                    }
                    if (possible)
                        pieces[v].y += 1;
                        soft_lock_clock.restart();
                        possible = false;
                    break;


            }

            // on update la grid
            for (int i=0;i<4;i++) {
                for (int j=0;j<4;j++) {
                    if (pieces[v].shape[4*i+j] == 1) {
                        this->cells[(pieces[v].y+i)*numcols + pieces[v].x+j] = 0; 
                    }
                }

            }
            
            // on vérifie si ce mouvement amene a ce que la piece soit immobilisée
            // si oui, on ajoute la piece à la grid
            // et on la retire des pieces qui tombent
            for (int i=0;i<4;i++) {
                for (int j=0;j<4;j++) {
                    if (pieces[v].shape[4*i+j] == 1 
                        && (this->cells[(pieces[v].y+i+1)*numcols + pieces[v].x+j] > 0
                        || pieces[v].y+i+1 >= numrows) && soft_lock_clock.getElapsedTime().asMilliseconds()>150) {
                        for (int k=0;k<4;k++) {
                            for (int l=0;l<4;l++) {
                                if (pieces[v].shape[4*k+l] == 1)
                                    cells[(pieces[v].y+k)*numcols+pieces[v].x+l] = pieces[v].type+1;
                        }
                            }
                        pieces.erase(pieces.begin()+i);
                        return;
                    }
                }
            }
        }
    }
}

/*On vérifie si un ligne est pleine*/

std::vector<int> Grid::check_lines() {
    std::vector<int> lines;
    for (int i=0;i<numrows;i++) {
        bool full = true;
        for (int j=0;j<numcols;j++) {       //on vient vérifier une à une les cellules de chaque ligne en partant du bas
            if (cells[i*numcols+j] <= 0) {
                full = false;               //si une cellule est vide on renvoie false
            }
        }
        if (full) {
            lines.push_back(i);
        }
    }
    return lines;
}

void Grid::clear_line(int line) {
    for (int i=line;i>0;i--) {
        for (int j=0;j<numcols;j++) {
            cells[i*numcols+j] = cells[(i-1)*numcols+j];
        }
    }
    for (int j=0;j<numcols;j++) {
        cells[j] = -1;
    }
}