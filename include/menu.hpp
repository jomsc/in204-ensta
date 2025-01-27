#ifndef MENU_HPP
#define MENU_HPP 

#include "button.hpp"

#include <vector>
#include <SFML/Graphics.hpp>
#include <atomic>
#include <thread>

class Menu {
    private:
        std::vector<Button> buttons;

    public:
        void display(sf::RenderWindow *window);

        // returns status that should be applied if isClicking is true, -1 otherwise
        int dest(int x, int y, bool isClicking); 

        Menu(std::vector<int*> infos_array, sf::Texture menuTexture) {
            for (auto infos : infos_array) {
                Button button = Button(infos, menuTexture);
                buttons.push_back(button);
            }
        }
};

#endif