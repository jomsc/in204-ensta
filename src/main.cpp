#include <SFML/Graphics.hpp>
#include "player.hpp"
#include "server.hpp"
#include <iostream>
#include <enet/enet.h>  
#include <chrono>



int main()
{

    sf::VideoMode ecran = sf::VideoMode::getDesktopMode();
    int VIDEO_WIDTH= ecran.width;
    int VIDEO_HEIGHT= ecran.height;

    sf::RenderWindow window(sf::VideoMode(VIDEO_WIDTH, VIDEO_HEIGHT), "Tetris!");
    srand(time(0));

    sf::Clock clock;

    Player player = Player();
    OnlinePlayer online_player = OnlinePlayer();

    int status = 2; // 0 : menu, 1 : classic game, 2 : LAN screen, 3 : LAN game

    player.grid.size_cell=(VIDEO_HEIGHT-2*player.grid.y_offset
        -player.grid.line_thickness*(player.grid.numrows+1))/(player.grid.numrows+1);
    player.grid.x_offset= (VIDEO_WIDTH-player.grid.numcols*player.grid.size_cell
        -(player.grid.numcols+1)*player.grid.line_thickness)/2;
    player.grid.size_waiting_cell = 0.625*player.grid.size_cell;
    
    sf::Texture bgVideoTexture;
    sf::Sprite bgVideoSprite;
    sf::Clock bgClock;
    bgClock.restart();
    int vidX[107];
    int vidY[107];
    float scaleX = VIDEO_WIDTH/1920.0;
    float scaleY = VIDEO_HEIGHT/1080.0;
    std::cout << scaleX<<std::endl;

    for (int i=0;i<13;i++){
        for (int j=0;j<8;j++) {
            vidX[8*i+j] = 1080*i;
            vidY[8*i+j] = 1920*j;
        }
    }

    for (int j=0;j<3;j++) {
        vidX[8*13+j] = 1080*13;
        vidY[8*13+j] = 1920*j;
    }

    if (!bgVideoTexture.loadFromFile("../src/assets/background_vid/out.jpeg")) {
        std::cout << "Error loading background video!" << std::endl;  
    }
    bgVideoSprite.setTexture(bgVideoTexture);
    bgVideoSprite.setScale(sf::Vector2f(scaleX, scaleY));

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