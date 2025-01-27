#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <SFML/Graphics.hpp>

class Button {
    private:
        int x;
        int y;
        int w;
        int h;
        sf::IntRect srcRect;
        sf::Sprite buttonSprite;
        int onClickDest;

    public:
        void display(sf::RenderWindow *window) {
            buttonSprite.setPosition(this->x, this->y);
            window->draw(this->buttonSprite);
        }

        bool isInside(int a, int b) {
            return (a >= x && a <= x+w) && (b >= y && y <= b+h);
        }

        int getDest() { return onClickDest; }

        Button(int infos[7], sf::Texture theTexture) {
            this->x = infos[0];
            this->y = infos[1];
            this->w = infos[2];
            this->h = infos[3];
            srcRect = sf::IntRect(infos[4], infos[5], this->w, this->h);
            this->onClickDest = infos[6];

            buttonSprite.setTexture(theTexture);
            buttonSprite.setTextureRect(this->srcRect); 
        }

        ~Button() {}

};

#endif