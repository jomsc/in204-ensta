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
    OnlinePlayer online_player = OnlinePlayer();
    
    int status = 2; // 0 : menu, 1 : classic game, 2 : LAN screen, 3 : LAN game
    

    sf::RectangleShape bg(sf::Vector2f(800, 1000));
    sf::Color bg_color = sf::Color(75, 75, 75);
    bg.setFillColor(bg_color);

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

        switch (status) {

            case 0:
                // menu
                // TO DO : display menu, handle mouse to select 
                break;

            case 1:
                player.update();
                window.clear();
                window.draw(bg);
                player.display(&window);
                window.display();
                break;

            case 2:
                if (discovery) {
                    std::vector<GameInfo> games = online_player.game_discovery.discoverGames(2000, 1);
                    for (const auto& game : games) {
                        std::cout << "Game found : " << std::endl;
                        std::cout << "Name : " << game.gameName << std::endl;
                        std::cout << "Port : " << game.gamePort << std::endl;
                        std::cout << "MOTD : " << game.motd << std::endl;
                        std::cout << std::endl;
                    }
                    std::cout << "choose a game (-1 to refuse)" << std::endl;
                    int game_chosen;
                    std::cin >> game_chosen;
                    
                    if (game_chosen!=-1 && game_chosen <= games.size()) {
                        std::string pseudo;
                        std::cout << "choose your pseudo (16 characters max)" << std::endl;
                        std::cin >> pseudo;
                        online_player.setPseudo(pseudo);
                        bool success = false;
                        success = online_player.connect_to_server(games[game_chosen]);
                        if (success) {
                            status = 3;
                        }
                    }
                } else {
                    std::cout << "server mode" << std::endl;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                break;
            
            case 3:
                break;
            
            default:
                break;

        }

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        
        //
        
        sf::Time elapsed = clock.getElapsedTime();
        //std::cout << "Render time : " << elapsed.asMilliseconds() << " ms" << std::endl;
    }

    return 0;
}