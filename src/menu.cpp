#include "menu.hpp"

void Menu::display(sf::RenderWindow *window, int a, int b) {
    for (auto button : this->buttons) {
        button.display(window, a, b);
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