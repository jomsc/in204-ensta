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
        void display(sf::RenderWindow *window, int a, int b);

        // returns status that should be applied if isClicking is true, -1 otherwise
        int dest(int x, int y, bool isClicking); 

        Menu(std::vector<std::vector<int>> infos_array, sf::Texture menuTexture,
             std::vector<sf::Color> colorArray, std::vector<sf::Color> hoverColorArray) {
            for (int i=0;i<infos_array.size();i++) {
                Button button = Button(infos_array[i], menuTexture, colorArray[i], hoverColorArray[i]);
                buttons.push_back(button);
            }
        }
};

#endif