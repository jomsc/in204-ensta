#include "grid.hpp"
#include <iterator>
#include <cstdlib>

void Grid::draw(sf::RenderWindow *window, int score, int level, int buffer[3]) {
    // on dessine la grille
   
    // on dessine les lignes horizontales
    for (int i=0;i<=numrows;i++) {
        int size = numcols*size_cell+(numcols)*line_thickness;
        sf::RectangleShape ligne(sf::Vector2f(size, line_thickness));
        ligne.setPosition(x_offset, y_offset+i*(size_cell+line_thickness));
        ligne.setFillColor(sf::Color::Black);
        window->draw(ligne);
    }

    // on dessine les lignes verticales
    for (int i=0;i<=numcols;i++) {
        int size = numrows*size_cell+(numrows+1)*line_thickness;
        sf::RectangleShape ligne(sf::Vector2f(size, line_thickness));
        ligne.setPosition(x_offset+i*(size_cell+line_thickness), y_offset);
        ligne.setFillColor(sf::Color::Black);
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
                           +line_thickness+x_offset+10, y_offset);
    window->draw(score_text);

    // on dessine le niveau 
    std::string levelstr = "Niveau : " + std::to_string(level);
    sf::Text level_text(levelstr, arial);
    level_text.setCharacterSize(30);
    level_text.setStyle(sf::Text::Bold);
    level_text.setFillColor(sf::Color::White);
    level_text.setPosition(numcols*(size_cell+line_thickness)
                           +line_thickness+x_offset+10, 2*y_offset+30);
    window->draw(level_text);

    //on dessine les 3 pièces suivantes
    for(int k=0;k<3;k++)
    {
        Piece waiting_piece(0,0,0,buffer[k],1);
        draw_waiting_piece(window, waiting_piece,k);
    }    
}

void Grid::draw_in_cell(sf::RenderWindow *window, int x, int y, sf::Color color) {
    sf::RectangleShape cell(sf::Vector2f(size_cell, size_cell));
    cell.setPosition(x_offset+x*(size_cell+line_thickness), 
        y_offset+y*(size_cell+line_thickness)+line_thickness);
    cell.setFillColor(color);
    window->draw(cell);
}

void Grid::draw_waiting_piece(sf::RenderWindow *window, Piece waiting_piece,int queue_pos) {
    sf::RectangleShape waiting_cell(sf::Vector2f(size_waiting_cell, size_waiting_cell));
    for(int i=0;i<4;i++)
    {
        for(int j=0; j<4; j++)
        {
            waiting_cell.setPosition(x_offset+numcols*(size_cell+line_thickness)+10+j*size_waiting_cell, 
                y_offset+100+queue_pos*100+i*size_waiting_cell);
            waiting_cell.setFillColor(waiting_piece.color);
            if(waiting_piece.shape[4*i+j]==1)
            {
                window->draw(waiting_cell);   
            }
        }
    }
}

void Grid::spawn(int type, bool* fin) {
    pieces.emplace_back(2, 0, 0, type, 1);

    for (int i=0;i<4;i++) {
        for (int j=0;j<4;j++) {
            if (pieces[pieces.size()-1].shape[4*i+j] == 1) {
                if(this->cells[(pieces[pieces.size()-1].y+i)*numcols+pieces[pieces.size()-1].x+j] <= 0)
                {
                   this->cells[(pieces[pieces.size()-1].y+i)*numcols+pieces[pieces.size()-1].x+j] = 0; 
                }
                else{
                    *fin=true;
                }
            }
        }
    }
}


void Grid::update(bool *floor, bool* left_wall, bool* right_wall, bool* lock_in) 
{

    if (!pieces.empty()) 
    {
        for (int v=0;v<pieces.size();v++) 
        {

        //on check s'il y a un mur à droite
        for (int i=0;i<4;i++) 
            {
                for (int j=0;j<4;j++) 
                {
                   if (pieces[v].shape[4*i+j] == 1) 
                    {
                        if (pieces[v].x+j >= numcols-1 ||
                            cells[(pieces[v].y+i)*numcols + pieces[v].x+j+1] > 0)
                        {
                            *right_wall=true;
                        }
                    }
                }
            }

        //on check s'il y a un mur à gauche
        for (int i=0;i<4;i++) 
            {
                for (int j=0;j<4;j++) 
                {
                   if (pieces[v].shape[4*i+j] == 1) 
                    {
                        if (pieces[v].x+j < 1 ||
                        cells[(pieces[v].y+i)*numcols + pieces[v].x+j-1] > 0)
                        {
                            *left_wall=true;
                        }
                    }
                }
            }



        //on check s'il un qqchose qui bloque en dessous
    for (int i=0;i<4;i++) 
        {
            for (int j=0;j<4;j++) 
            {
                if (pieces[v].shape[4*i+j] == 1 
                    && (this->cells[(pieces[v].y+i+1)*numcols + pieces[v].x+j] > 0
                    || pieces[v].y+i+1 >= numrows)) 
                {
                    *floor=true;
                }
            }
        }

        //si oui on lock la pièce
    if(*lock_in)
    {
        *lock_in=false;
            for (int i=0;i<4;i++) 
            {
                for (int j=0;j<4;j++) 
                {
                    for (int k=0;k<4;k++) 
                    {
                        for (int l=0;l<4;l++) 
                        {
                            if (pieces[v].shape[4*k+l] == 1)
                            {
                                cells[(pieces[v].y+k)*numcols+pieces[v].x+l] = pieces[v].type+1;
                            }
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

bool Grid::impossible_kick(int x1,int y1,int sens)
{
    int vrai_sens;
    if(sens==1)
    {
        vrai_sens=0;
    }
    else
    {
        vrai_sens=1;
    }

    int y2=-y1;

    std::vector<Piece> temp = pieces;
    temp[0].rotate(vrai_sens);

    for(int i=0;i<abs(x1);i++)
    {
        if(x1<0)
        {
            temp[0].move(0);
        }
        else
        {
            temp[0].move(1);
        }
    }

    for(int i=0;i<abs(y2);i++)
    {
        if(y2<0)
        {
            temp[0].move(3);
        }
        else
        {
            temp[0].move(2);
        }
    }

        for(int i=0;i<4;i++)
        {
            for(int j=0;j<4;j++)
            {
                if(temp[0].shape[4*i+j]==1)
                {
                    if(this->cells[(temp[0].y+i)*numcols+temp[0].x+j]>0||
                      temp[0].x+j < 0 ||
                      temp[0].x+j>=numcols ||
                      temp[0].y+i>=numrows)
                    {   
                        return true;
                    }
                }
            }
        }
    return false;    
}

void Grid::wall_kick_clockwise(int type,int orientation)
{
    for(int v=0;v<pieces.size();v++)
    {
        switch(orientation)
        {
            case 0:
                switch(type)
                {
                    case 0:
                        return;
                    case 1:
                        if(!impossible_kick(0,0,1))
                        {
                            pieces[v].x+= 0;
                            pieces[v].y+= 0;
                            return;
                        }
                        else if(!impossible_kick(-2,0,1))
                        {
                            pieces[v].x+= -2;
                            pieces[v].y+= 0;
                            return;
                        }
                        else if(!impossible_kick(1,0,1))
                        {
                            pieces[v].x+= 1;
                            pieces[v].y+= 0;
                            return;
                        }
                        else if(!impossible_kick(-2,-1,1))
                        {
                            pieces[v].x+= -2;
                            pieces[v].y+= 1;
                            return;
                        }
                        else if(!impossible_kick(1,2,1))
                        {
                            pieces[v].x+= 1;
                            pieces[v].y+= -2;
                            return;
                        }
                        else
                        {
                            return;
                        }
                    case 2:
                        goto case60;
                    case 3:
                        goto case60;
                    case 4:
                        goto case60;
                    case 5:
                        goto case60;
                    case 6:
                    case60:
                        if(!impossible_kick(0,0,1))
                        {
                            pieces[v].x+= 0;
                            pieces[v].y+= 0;
                            return;
                        }
                        else if(!impossible_kick(-1,0,1))
                        {
                            pieces[v].x+= -1;
                            pieces[v].y+= 0;
                            return;
                        }
                        else if(!impossible_kick(-1,1,1))
                        {
                            pieces[v].x+= -1;
                            pieces[v].y+= -1;
                            return;
                        }
                        else if(!impossible_kick(0,-2,1))
                        {
                            pieces[v].x+= 0;
                            pieces[v].y+= 2;
                            return;
                        }
                        else if(!impossible_kick(-1,-2,1))
                        {
                            pieces[v].x+= -1;
                            pieces[v].y+= 2;
                            return;
                        }
                        else
                        {
                            return;
                        }
                }
            case 1:
                switch(type)
                {
                    case 0:
                        return;
                    case 1:
                        if(!impossible_kick(0,0,1))
                        {
                            pieces[v].x+= 0;
                            pieces[v].y+= 0;
                            return;
                        }
                        else if(!impossible_kick(-1,0,1))
                        {
                            pieces[v].x+= -1;
                            pieces[v].y+= 0;
                            return;
                        }
                        else if(!impossible_kick(2,0,1))
                        {
                            pieces[v].x+= 2;
                            pieces[v].y+= 0;
                            return;
                        }
                        else if(!impossible_kick(-1,2,1))
                        {
                            pieces[v].x+= -1;
                            pieces[v].y+= -2;
                            return;
                        }
                        else if(!impossible_kick(2,-1,1))
                        {
                            pieces[v].x+= 2;
                            pieces[v].y+= 1;
                            return;
                        }
                        else
                        {
                            return;
                        }
                    case 2:
                        goto case61;
                    case 3:
                        goto case61;
                    case 4:
                        goto case61;
                    case 5:
                        goto case61;
                    case 6:
                    case61:
                        if(!impossible_kick(0,0,1))
                        {
                            pieces[v].x+= 0;
                            pieces[v].y+= 0;
                            return;
                        }
                        else if(!impossible_kick(1,0,1))
                        {
                            pieces[v].x+= 1;
                            pieces[v].y+= 0;
                            return;
                        }
                        else if(!impossible_kick(1,-1,1))
                        {
                            pieces[v].x+= 1;
                            pieces[v].y+= 1;
                            return;
                        }
                        else if(!impossible_kick(0,2,1))
                        {
                            pieces[v].x+= 0;
                            pieces[v].y+= -2;
                            return;
                        }
                        else if(!impossible_kick(1,2,1))
                        {
                            pieces[v].x+= 1;
                            pieces[v].y+= -2;
                            return;
                        }
                        else
                        {
                            return;
                        }
                }
            case 2:
                switch(type)
                {
                    case 0:
                        return;
                    case 1:
                        if(!impossible_kick(0,0,1))
                        {
                            pieces[v].x+= 0;
                            pieces[v].y+= 0;
                            return;
                        }
                        else if(!impossible_kick(2,0,1))
                        {
                            pieces[v].x+= 2;
                            pieces[v].y+= 0;
                            return;
                        }
                        else if(!impossible_kick(-1,0,1))
                        {
                            pieces[v].x+= -1;
                            pieces[v].y+= 0;
                            return;
                        }
                        else if(!impossible_kick(2,1,1))
                        {
                            pieces[v].x+= 2;
                            pieces[v].y+= -1;
                            return;
                        }
                        else if(!impossible_kick(-1,-2,1))
                        {
                            pieces[v].x+= -1;
                            pieces[v].y+= 2;
                            return;
                        }
                        else
                        {
                            return;
                        }
                    case 2:
                        goto case62;
                    case 3:
                        goto case62;
                    case 4:
                        goto case62;
                    case 5:
                        goto case62;
                    case 6:
                    case62:
                        if(!impossible_kick(0,0,1))
                        {
                            pieces[v].x+= 0;
                            pieces[v].y+= 0;
                            return;
                        }
                        else if(!impossible_kick(1,0,1))
                        {
                            pieces[v].x+= 1;
                            pieces[v].y+= 0;
                            return;
                        }
                        else if(!impossible_kick(1,1,1))
                        {
                            pieces[v].x+= 1;
                            pieces[v].y+= -1;
                            return;
                        }
                        else if(!impossible_kick(0,-2,1))
                        {
                            pieces[v].x+= 0;
                            pieces[v].y+= 2;
                            return;
                        }
                        else if(!impossible_kick(1,-2,1))
                        {
                            pieces[v].x+= 1;
                            pieces[v].y+= 2;
                            return;
                        }
                        else
                        {
                            return;
                        }
                }
            case 3:
                switch(type)
                {
                    case 0:
                        return;
                    case 1:
                        if(!impossible_kick(0,0,1))
                        {
                            pieces[v].x+= 0;
                            pieces[v].y+= 0;
                            return;
                        }
                        else if(!impossible_kick(1,0,1))
                        {
                            pieces[v].x+= 1;
                            pieces[v].y+= 0;
                            return;
                        }
                        else if(!impossible_kick(-2,0,1))
                        {
                            pieces[v].x+= -2;
                            pieces[v].y+= 0;
                            return;
                        }
                        else if(!impossible_kick(1,-2,1))
                        {
                            pieces[v].x+= 1;
                            pieces[v].y+= 2;
                            return;
                        }
                        else if(!impossible_kick(-2,1,1))
                        {
                            pieces[v].x+= -2;
                            pieces[v].y+= -1;
                            return;
                        }
                        else
                        {
                            return;
                        }
                    case 2:
                        goto case63;
                    case 3:
                        goto case63;
                    case 4:
                        goto case63;
                    case 5:
                        goto case63;
                    case 6:
                    case63:
                        if(!impossible_kick(0,0,1))
                        {
                            pieces[v].x+= 0;
                            pieces[v].y+= 0;
                            return;
                        }
                        else if(!impossible_kick(-1,0,1))
                        {
                            pieces[v].x+= -1;
                            pieces[v].y+= 0;
                            return;
                        }
                        else if(!impossible_kick(-1,-1,1))
                        {
                            pieces[v].x+= -1;
                            pieces[v].y+= 1;
                            return;
                        }
                        else if(!impossible_kick(0,2,1))
                        {
                            pieces[v].x+= 0;
                            pieces[v].y+= -2;
                            return;
                        }
                        else if(!impossible_kick(-1,2,1))
                        {
                            pieces[v].x+= -1;
                            pieces[v].y+= -2;
                            return;
                        }
                        else
                        {
                            return;
                        }
                }
        }
    }
}

void Grid::wall_kick_counterclockwise(int type,int orientation)
{
    for(int v=0;v<pieces.size();v++)
    {
        switch(orientation)
        {
            case 0:
                switch(type)
                {
                    case 0:
                        return;
                    case 1:
                        if(!impossible_kick(0,0,3))
                        {
                            pieces[v].x+= 0;
                            pieces[v].y+= 0;
                            return;
                        }
                        else if(!impossible_kick(-1,0,3))
                        {
                            pieces[v].x+= -1;
                            pieces[v].y+= 0;
                            return;
                        }
                        else if(!impossible_kick(2,0,3))
                        {
                            pieces[v].x+= 2;
                            pieces[v].y+= 0;
                            return;
                        }
                        else if(!impossible_kick(-1,2,3))
                        {
                            pieces[v].x+= -1;
                            pieces[v].y+= -2;
                            return;
                        }
                        else if(!impossible_kick(2,-1,3))
                        {
                            pieces[v].x+= 2;
                            pieces[v].y+= 1;
                            return;
                        }
                        else
                        {
                            return;
                        }
                    case 2:
                        goto case64;
                    case 3:
                        goto case64;
                    case 4:
                        goto case64;
                    case 5:
                        goto case64;
                    case 6:
                    case64:
                        if(!impossible_kick(0,0,3))
                        {
                            pieces[v].x+= 0;
                            pieces[v].y+= 0;
                            return;
                        }
                        else if(!impossible_kick(1,0,3))
                        {
                            pieces[v].x+= 1;
                            pieces[v].y+= 0;
                            return;
                        }
                        else if(!impossible_kick(1,1,3))
                        {
                            pieces[v].x+= 1;
                            pieces[v].y+= -1;
                            return;
                        }
                        else if(!impossible_kick(0,-2,3))
                        {
                            pieces[v].x+= 0;
                            pieces[v].y+= 2;
                            return;
                        }
                        else if(!impossible_kick(1,-2,3))
                        {
                            pieces[v].x+= 1;
                            pieces[v].y+= 2;
                            return;
                        }
                        else
                        {
                            return;
                        }
                }
            case 1:
                switch(type)
                {
                    case 0:
                        return;
                    case 1:
                        if(!impossible_kick(0,0,3))
                        {
                            pieces[v].x+= 0;
                            pieces[v].y+= 0;
                            return;
                        }
                        else if(!impossible_kick(2,0,3))
                        {
                            pieces[v].x+= 2;
                            pieces[v].y+= 0;
                            return;
                        }
                        else if(!impossible_kick(-1,0,3))
                        {
                            pieces[v].x+= -1;
                            pieces[v].y+= 0;
                            return;
                        }
                        else if(!impossible_kick(2,1,3))
                        {
                            pieces[v].x+= 2;
                            pieces[v].y+= -1;
                            return;
                        }
                        else if(!impossible_kick(-1,-2,3))
                        {
                            pieces[v].x+= -1;
                            pieces[v].y+= 2;
                            return;
                        }
                        else
                        {
                            return;
                        }
                    case 2:
                        goto case65;
                    case 3:
                        goto case65;
                    case 4:
                        goto case65;
                    case 5:
                        goto case65;
                    case 6:
                    case65:
                        if(!impossible_kick(0,0,3))
                        {
                            pieces[v].x+= 0;
                            pieces[v].y+= 0;
                            return;
                        }
                        else if(!impossible_kick(1,0,3))
                        {
                            pieces[v].x+= 1;
                            pieces[v].y+= 0;
                            return;
                        }
                        else if(!impossible_kick(1,-1,3))
                        {
                            pieces[v].x+= 1;
                            pieces[v].y+= 1;
                            return;
                        }
                        else if(!impossible_kick(0,2,3))
                        {
                            pieces[v].x+= 0;
                            pieces[v].y+= -2;
                            return;
                        }
                        else if(!impossible_kick(1,2,3))
                        {
                            pieces[v].x+= 1;
                            pieces[v].y+= -2;
                            return;
                        }
                        else
                        {
                            return;
                        }
                }
            case 2:
                switch(type)
                {
                    case 0:
                        return;
                    case 1:
                        if(!impossible_kick(0,0,3))
                        {
                            pieces[v].x+= 0;
                            pieces[v].y+= 0;
                            return;
                        }
                        else if(!impossible_kick(1,0,3))
                        {
                            pieces[v].x+= 1;
                            pieces[v].y+= 0;
                            return;
                        }
                        else if(!impossible_kick(-2,0,3))
                        {
                            pieces[v].x+= -2;
                            pieces[v].y+= 0;
                            return;
                        }
                        else if(!impossible_kick(1,-2,3))
                        {
                            pieces[v].x+= 1;
                            pieces[v].y+= 2;
                            return;
                        }
                        else if(!impossible_kick(-2,1,3))
                        {
                            pieces[v].x+= -2;
                            pieces[v].y+= -1;
                            return;
                        }
                        else
                        {
                            return;
                        }
                    case 2:
                        goto case66;
                    case 3:
                        goto case66;
                    case 4:
                        goto case66;
                    case 5:
                        goto case66;
                    case 6:
                    case66:
                        if(!impossible_kick(0,0,3))
                        {
                            pieces[v].x+= 0;
                            pieces[v].y+= 0;
                            return;
                        }
                        else if(!impossible_kick(-1,0,3))
                        {
                            pieces[v].x+= -1;
                            pieces[v].y+= 0;
                            return;
                        }
                        else if(!impossible_kick(-1,1,3))
                        {
                            pieces[v].x+= -1;
                            pieces[v].y+= -1;
                            return;
                        }
                        else if(!impossible_kick(0,-2,3))
                        {
                            pieces[v].x+= 0;
                            pieces[v].y+= 2;
                            return;
                        }
                        else if(!impossible_kick(-1,-2,3))
                        {
                            pieces[v].x+= -1;
                            pieces[v].y+= 2;
                            return;
                        }
                        else
                        {
                            return;
                        }
                }
            case 3:
                switch(type)
                {
                    case 0:
                        return;
                    case 1:
                        if(!impossible_kick(0,0,3))
                        {
                            pieces[v].x+= 0;
                            pieces[v].y+= 0;
                            return;
                        }
                        else if(!impossible_kick(-2,0,3))
                        {
                            pieces[v].x+= -2;
                            pieces[v].y+= 0;
                            return;
                        }
                        else if(!impossible_kick(1,0,3))
                        {
                            pieces[v].x+= 1;
                            pieces[v].y+= 0;
                            return;
                        }
                        else if(!impossible_kick(-2,-1,3))
                        {
                            pieces[v].x+= -2;
                            pieces[v].y+= 1;
                            return;
                        }
                        else if(!impossible_kick(1,2,3))
                        {
                            pieces[v].x+= 1;
                            pieces[v].y+= -2;
                            return;
                        }
                        else
                        {
                            return;
                        }
                    case 2:
                        goto case67;
                    case 3:
                        goto case67;
                    case 4:
                        goto case67;
                    case 5:
                        goto case67;
                    case 6:
                    case67:
                        if(!impossible_kick(0,0,3))
                        {
                            pieces[v].x+= 0;
                            pieces[v].y+= 0;
                            return;
                        }
                        else if(!impossible_kick(-1,0,3))
                        {
                            pieces[v].x+= -1;
                            pieces[v].y+= 0;
                            return;
                        }
                        else if(!impossible_kick(-1,-1,3))
                        {
                            pieces[v].x+= -1;
                            pieces[v].y+= 1;
                            return;
                        }
                        else if(!impossible_kick(0,2,3))
                        {
                            pieces[v].x+= 0;
                            pieces[v].y+= -2;
                            return;
                        }
                        else if(!impossible_kick(-1,2,3))
                        {
                            pieces[v].x+= -1;
                            pieces[v].y+= -2;
                            return;
                        }
                        else
                        {
                            return;
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