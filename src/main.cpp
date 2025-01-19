#include <SFML/Graphics.hpp>
#include "player.hpp"
#include <iostream>
#include <enet/enet.h>  

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 1000), "Tetris!");
    srand(time(0));

    sf::Clock clock;

    Player player = Player();
    
    

    sf::RectangleShape bg(sf::Vector2f(800, 1000));
    sf::Color bg_color = sf::Color(75, 75, 75);
    bg.setFillColor(bg_color);

    while (window.isOpen()) {
        clock.restart();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        
        player.update();

        window.clear();
        window.draw(bg);
        player.display(&window);
        window.display();
        
        sf::Time elapsed = clock.getElapsedTime();
        //std::cout << "Render time : " << elapsed.asMilliseconds() << " ms" << std::endl;
    }

    return 0;
}