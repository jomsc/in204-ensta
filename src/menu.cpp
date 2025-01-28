#include "menu.hpp"

void Menu::display(sf::RenderWindow *window, int a, int b) {
    for (auto button : this->buttons) {
        button.display(window, a, b);
    }
}

void Menu::add_button(std::vector<int> infos, sf::Color color, 
                      sf::Color onHoverColor, sf::Texture theTexture) {
    
    Button button = Button(infos, theTexture, color, onHoverColor);
    buttons.push_back(button);
}

int Menu::dest(int x, int y, bool isClicking) {
    if (isClicking) {
        for (auto button : this->buttons) {
            if (button.isInside(x, y)) {
                return button.getDest();
            }
        }
        return -1;
    } else { return -1; }
}