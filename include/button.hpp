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
        int onClickDest; // -1 if not clickable
        sf::Color buttonColor;
        sf::Color onHoverColor;
        sf::Texture buttonTexture;

    public:
        void display(sf::RenderWindow *window, int a, int b) {
            if (isInside(a, b) && (onClickDest != -1)) {
                buttonSprite.setColor(onHoverColor);
            } else {
                buttonSprite.setColor(buttonColor);
            }
            
            window->draw(this->buttonSprite);
        }

        bool isInside(int a, int b) {
            return ((a >= x) && (a <= x+w)) && ((b >= y) && (b <= y+h));
        }

        int getDest() { return onClickDest; }

        Button(std::vector<int> infos, sf::Texture theTexture, sf::Color theColor, sf::Color theHoverColor) {
            this->x = infos[0];
            this->y = infos[1];
            this->w = infos[2];
            this->h = infos[3];
            srcRect = sf::IntRect(infos[4], infos[5], this->w, this->h);
            this->onClickDest = infos[6];

            buttonTexture = theTexture;
            buttonSprite.setTexture(buttonTexture);
            buttonSprite.setTextureRect(this->srcRect); 
            buttonSprite.setPosition(this->x, this->y);

            buttonColor = theColor;
            onHoverColor = theHoverColor;
        }

        ~Button() {}

};

#endif