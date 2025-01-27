#include "menu.hpp"

void Menu::display(sf::RenderWindow *window) {
    for (auto button : this->buttons) {
        button.display(window);
    }
}

int Menu::dest(int x, int y, bool isClicking) {
    if (isClicking) {
        for (auto button : this->buttons) {
            if (button.isInside(x, y)) {
                return button.getDest();
            }
        }
    } else { return -1; }
}