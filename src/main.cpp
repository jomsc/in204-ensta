#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "../include/piece.hpp"
#include "../include/grid.hpp"

#include <cstdlib>
#include <iostream>


int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 1000), "Tetris!");
    srand(time(0));

    sf::Clock clock;
    sf::Clock update_clock;
    sf::Clock rotate_clock;

    Grid grid = Grid();
    grid.spawn(rand()%7);
    
    int input = -1;

    sf::RectangleShape bg(sf::Vector2f(800, 1000));
    sf::Color bg_color = sf::Color(75, 75, 75);
    bg.setFillColor(bg_color);

    update_clock.restart();
    rotate_clock.restart();
    while (window.isOpen()) {
        clock.restart();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            input = 0;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            input = 1;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            input = 2;
        } else  {
            input = -1;
        }
    
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::R) && rotate_clock.getElapsedTime().asMilliseconds() > 300) {
            grid.pieces[0].rotate();
            rotate_clock.restart();
        } 

        if ((input == 0 || input == 1) 
            && update_clock.getElapsedTime().asMilliseconds() > 500) {
            grid.update(input);
            grid.update(2);
            update_clock.restart();
        } else if (input == 2 && update_clock.getElapsedTime().asMilliseconds() > 150) {
            grid.update(2);
            update_clock.restart();
        } else if (update_clock.getElapsedTime().asMilliseconds() > 500) {
            grid.update(2);
            update_clock.restart();
        }

        if (grid.pieces.empty()) {
            grid.spawn(rand()%7);
        }

        std::vector<int> lines = grid.check_lines();
        if (!lines.empty()) {
            for (auto line : lines) {
                grid.clear_line(line);
            }
        }
        
        window.clear();
        window.draw(bg);
        grid.draw(&window);
        window.display();
        
        sf::Time elapsed = clock.getElapsedTime();
        //std::cout << "Render time : " << elapsed.asMilliseconds() << " ms" << std::endl;
    }

    return 0;
}