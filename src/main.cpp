#include <SFML/Graphics.hpp>
#include "player.hpp"
#include "server.hpp"
#include <iostream>
#include <enet/enet.h>  
#include <chrono>

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 1000), "Tetris!");
    srand(time(0));

    sf::Clock clock;

    Player player = Player();
    
    

    sf::RectangleShape bg(sf::Vector2f(800, 1000));
    sf::Color bg_color = sf::Color(0, 0, 50);
    bg.setFillColor(bg_color);

    OnlinePlayer online_player = OnlinePlayer();
    GameServer game_server = GameServer("zizi", "cacarthur bouvet", 
                                        25565, 2);
    bool host = true; 
    bool discovery = false;

    if (host) {
        game_server.create_game();
        std::cout << "Game created" << std::endl;
    }


    while (window.isOpen()) {
        clock.restart();

        if (discovery) {
            
            std::vector<GameInfo> games = online_player.game_discovery.discoverGames();
            for (const auto& game : games) {
                std::cout << "Game found : " << std::endl;
                std::cout << "Name : " << game.gameName << std::endl;
                std::cout << "Port : " << game.gamePort << std::endl;
                std::cout << "MOTD : " << game.motd << std::endl;
                std::cout << std::endl;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }

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